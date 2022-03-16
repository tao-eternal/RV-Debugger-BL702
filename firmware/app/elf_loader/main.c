/**
 * @file main.c
 * @brief
 *
 * Copyright (c) 2021 Bouffalolab team
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 */
#include "main.h"

#include "elfloader/elfloader_fcn.h"
#include "ff.h"
#include "shell.h"

static void usbd_cdc_acm_bulk_out(uint8_t ep);
static void usbd_cdc_acm_bulk_in(uint8_t ep);
static void cdc_acm_printf(char *fmt, ...);

static FATFS fs;
static struct device *usb_fs;

static usbd_class_t cdc_class;
static usbd_interface_t cdc_cmd_intf;
static usbd_interface_t cdc_data_intf;
static usbd_endpoint_t cdc_out_ep = {.ep_addr = CDC_OUT_EP,
                                     .ep_cb = usbd_cdc_acm_bulk_out};
static usbd_endpoint_t cdc_in_ep = {.ep_addr = CDC_IN_EP,
                                    .ep_cb = usbd_cdc_acm_bulk_in};

extern USB_DESC_SECTION const uint8_t cdc_msc_descriptor[];
extern struct device *usb_dc_init(void);
extern void fatfs_usb_driver_register(void);
extern struct heap_info mmheap_root;

int main(void) {
  // bflb_platform_print_set(1);
  bflb_platform_init(0);

  shell_init();
  shell_set_print(cdc_acm_printf);
  fatfs_usb_driver_register();

  usbd_desc_register(cdc_msc_descriptor);
  usbd_cdc_add_acm_interface(&cdc_class, &cdc_cmd_intf);
  usbd_cdc_add_acm_interface(&cdc_class, &cdc_data_intf);
  usbd_interface_add_endpoint(&cdc_data_intf, &cdc_out_ep);
  usbd_interface_add_endpoint(&cdc_data_intf, &cdc_in_ep);
  usbd_msc_class_init(MSC_OUT_EP, MSC_IN_EP);

  usb_fs = usb_dc_init();
  if (usb_fs) {
    device_control(usb_fs, DEVICE_CTRL_SET_INT,
                   (void *)(USB_EP1_DATA_OUT_IT | USB_EP4_DATA_OUT_IT |
                            USB_EP5_DATA_IN_IT));
  }
  while (!usb_device_is_configured()) {
  }
  /*mount*/
  f_mount(&fs, "2:", 1);
  f_chdrive("2:");
  struct heap_state state;
  while (1) {
    mmheap_get_state(&mmheap_root, &state);
    MSG("remain size: %dKB\r\n", state.remain_size / 1024);
    bflb_platform_delay_ms(1000);
  }
  /*unmount*/
  f_mount(NULL, "2:", 1);
}

void rust_console_putbytes(const uint8_t *bs, const size_t len) {
  usbd_ep_write(CDC_IN_EP, bs, len, NULL);
}
uint8_t *rust_aligned_alloc(const size_t alignment, const size_t size) {
  return (uint8_t *)mmheap_align_alloc(&mmheap_root, alignment, size);
}
void rust_free(const uint8_t *ptr) { mmheap_free(&mmheap_root, ptr); }

/////////////////////////////////////
//////command functions start////////
/////////////////////////////////////

int modtest(int argc, char *argv[]) {
  uint8_t *ReadBuffer = NULL;
  // uint8_t *Text = NULL;
  FIL fnew;
  UINT fnum;
  FRESULT res_sd = 0;
  if (argc < 2) return -1;

  res_sd = f_open(&fnew, argv[1], FA_OPEN_EXISTING | FA_READ);
  if (res_sd != FR_OK) {
    cdc_acm_printf("open error\r\n");
    return -1;
  }
  ReadBuffer = (uint8_t *)malloc(fnew.obj.objsize);

  res_sd = f_read(&fnew, ReadBuffer, fnew.obj.objsize, &fnum);
  // use ReadBuffer
  // Text = (uint8_t *)rust_aligned_alloc(2, 0xda);
  // arch_memcpy(Text, ReadBuffer + 0x34, 0xda);
  // Text = ReadBuffer + 0x34;
  void *module = rust_elf_load(ReadBuffer);
  free(ReadBuffer);
  f_close(&fnew);

  typedef int (*add_t)(int, int);
  typedef int (*fib_t)(int);
  typedef int (*hello_t)(void);
  typedef int (*test_datarel_t)(void);

  add_t add = (add_t)rust_elf_sym(module, "add");
  fib_t fib = (fib_t)rust_elf_sym(module, "fib");
  hello_t hello = (hello_t)rust_elf_sym(module, "hello");
  test_datarel_t test_datarel = (test_datarel_t)rust_elf_sym(module,"test_datarel");

  // add_t add = (add_t)(Text);
  // fib_t fib = (fib_t)(Text + 0x20);
  // {
  //   // BRANCH
  //   uint32_t *location = (uint32_t *)(Text + 0x34);
  //   ssize_t offset = 0x3c - 0x34;
  //   uint32_t imm12, imm11, imm10_5, imm4_1;
  //   imm12 = (offset & 0x1000) << (31 - 12);
  //   imm11 = (offset & 0x800) >> (11 - 7);
  //   imm10_5 = (offset & 0x7e0) << (30 - 10);
  //   imm4_1 = (offset & 0x1e) << (11 - 4);
  //   cdc_acm_printf("0x34 BRANCH @%08x\r\n", location);
  //   *location &= 0x1fff07f;
  //   *location |= imm12 | imm11 | imm10_5 | imm4_1;
  //   cdc_acm_printf("0x34 BRANCH finished\r\n");
  // }
  // {
  //   // RVC_JUMP
  //   uint16_t *location = (uint16_t *)(Text + 0x3a);
  //   ssize_t offset = 0x62 - 0x3a;
  //   uint16_t imm11, imm10, imm9_8, imm7, imm6, imm5, imm4, imm3_1;
  //   imm11 = (offset & 0x800) << (12 - 11);
  //   imm10 = (offset & 0x400) >> (10 - 8);
  //   imm9_8 = (offset & 0x300) << (12 - 11);
  //   imm7 = (offset & 0x80) >> (7 - 6);
  //   imm6 = (offset & 0x40) << (12 - 11);
  //   imm5 = (offset & 0x20) >> (5 - 2);
  //   imm4 = (offset & 0x10) << (12 - 5);
  //   imm3_1 = (offset & 0xe) << (12 - 10);
  //   cdc_acm_printf("0x3a RVC_JUMP @%08x\r\n", location);
  //   *location &= 0xe003;
  //   *location |= imm11 | imm10 | imm9_8 | imm7 | imm6 | imm5 | imm4 |
  // imm3_1;
  //   cdc_acm_printf("0x3a RVC_JUMP finished\r\n");
  // }
  // {
  //   // CALL
  //   uint32_t *location = (uint32_t *)(Text + 0x44);
  //   ssize_t offset = 0x20 - 0x44;
  //   uint32_t hi20, lo12;

  //   hi20 = (offset + 0x800) & 0xfffff000;
  //   lo12 = (offset - hi20) & 0xfff;
  //   cdc_acm_printf("0x44 CALL @%08x\r\n", location);
  //   if ((uint32_t)location % 4) {
  //     uint16_t *loc = (uint16_t *)location;
  //     loc[0] &= 0xfff;
  //     loc[1] &= 0x0;
  //     loc[0] |= 0xffff & hi20;
  //     loc[1] |= hi20 >> 16;

  //     loc[2] &= 0xffff;
  //     loc[3] &= 0xf;
  //     loc[2] |= 0xffff & (lo12 << 20);
  //     loc[3] |= (lo12 << 20) >> 16;
  //   } else {
  //     *location &= 0xfff;
  //     *location |= hi20;
  //     *(location + 1) &= 0xfffff;
  //     *(location + 1) |= (lo12 << 20);
  //   }
  //   cdc_acm_printf("0x44 CALL finished\r\n");
  // }
  // {
  //   // CALL
  //   uint32_t *location = (uint32_t *)(Text + 0x56);
  //   ssize_t offset = 0x20 - 0x56;
  //   uint32_t hi20, lo12;

  //   hi20 = (offset + 0x800) & 0xfffff000;
  //   lo12 = (offset - hi20) & 0xfff;
  //   cdc_acm_printf("0x56 CALL @%08x\r\n", location);
  //   if ((uint32_t)location % 4) {
  //     uint16_t *loc = (uint16_t *)location;
  //     loc[0] &= 0xfff;
  //     loc[1] &= 0x0;
  //     loc[0] |= 0xffff & hi20;
  //     loc[1] |= hi20 >> 16;

  //     loc[2] &= 0xffff;
  //     loc[3] &= 0xf;
  //     loc[2] |= 0xffff & (lo12 << 20);
  //     loc[3] |= (lo12 << 20) >> 16;
  //   } else {
  //     *location &= 0xfff;
  //     *location |= hi20;
  //     *(location + 1) &= 0xfffff;
  //     *(location + 1) |= (lo12 << 20);
  //   }

  //   cdc_acm_printf("0x56 CALL finished\r\n");
  // }
  // test
  cdc_acm_printf("add(10,12)= %d\r\n", add(10, 22));
  cdc_acm_printf("fib(15)= %d\r\n", fib(15));
  cdc_acm_printf("hello()\r\n");
  hello();
  cdc_acm_printf("test_datarel()\r\n");
  test_datarel();

  // rust_free(Text);
  rust_elf_unload(module);
  return 0;
}
SHELL_CMD_EXPORT(modtest, a simple test)

int modload(int argc, char *argv[]) {
  uint8_t *ReadBuffer = NULL;
  // uint8_t *Text = NULL;
  FIL fnew;
  UINT fnum;
  FRESULT res_sd = 0;
  if (argc < 2) {
    cdc_acm_printf("Usage: modload <file>\r\n");
    return -1;
  }

  res_sd = f_open(&fnew, argv[1], FA_OPEN_EXISTING | FA_READ);
  if (res_sd != FR_OK) {
    cdc_acm_printf("open error\r\n");
    return -1;
  }
  ReadBuffer = (uint8_t *)malloc(fnew.obj.objsize);
  res_sd = f_read(&fnew, ReadBuffer, fnew.obj.objsize, &fnum);
  void *module = rust_elf_load(ReadBuffer);
  free(ReadBuffer);
  f_close(&fnew);

  if (module != NULL) {
    cdc_acm_printf("load success! Handle is %p\r\n", module);
  }

  return module;
}
SHELL_CMD_EXPORT(modload, load obj.o file from fatfs)

int modsym(int argc, char *argv[]) {
  if (argc < 3) {
    cdc_acm_printf("Usage: modsym <handle> <symname>\r\n");
    cdc_acm_printf("Tip: find from global if handle is 0\r\n");
    return -1;
  }

  void *sym = rust_elf_sym(strtol(argv[1], NULL, 16), argv[2]);
  if (sym != NULL) {
    cdc_acm_printf("symbol has found! Handle is %p\r\n", sym);
  }

  return sym;
}
SHELL_CMD_EXPORT(modsym, get sym from module handle or global with zero)

int modunload(int argc, char *argv[]) {
  if (argc < 2) {
    cdc_acm_printf("Usage: modunload <handle>\r\n");
    return -1;
  }

  rust_elf_unload(strtol(argv[1], NULL, 16));

  return 0;
}
SHELL_CMD_EXPORT(modunload, unload module with handle)

int dump(int argc, char *argv[]) {
  uint8_t *ReadBuffer = NULL;
  FIL fnew;
  UINT fnum;
  FRESULT res_sd = 0;
  if (argc < 2) return -1;

  res_sd = f_open(&fnew, argv[1], FA_OPEN_EXISTING | FA_READ);
  if (res_sd != FR_OK) {
    cdc_acm_printf("open error\r\n");
    return -1;
  }
  ReadBuffer = (uint8_t *)malloc(fnew.obj.objsize);

  res_sd = f_read(&fnew, ReadBuffer, fnew.obj.objsize, &fnum);
  // bflb_platform_dump(ReadBuffer, fnum);
  for (size_t i = 0; i < fnum; i++) {
    if (i % 16 == 0) {
      cdc_acm_printf("\r\n");
    }
    cdc_acm_printf("%02x ", ReadBuffer[i]);
  }
  cdc_acm_printf("\r\n");

  free(ReadBuffer);
  f_close(&fnew);
  return 0;
}
SHELL_CMD_EXPORT(dump, dump file from fatfs)

static int ota(int argc, char *argv[]) {
  hal_enter_usb_iap();
  return 0;
}
SHELL_CMD_EXPORT(ota, enter ota)

/////////////////////////////////////
//////static functions start/////////
/////////////////////////////////////
static void usbd_cdc_acm_bulk_out(uint8_t ep) {
  uint32_t actual_read_length = 0;
  uint8_t out_buffer[64];

  if (usbd_ep_read(ep, out_buffer, 64, &actual_read_length) < 0) {
    MSG("Read DATA Packet failed\r\n");
    usbd_ep_set_stall(ep);
    return;
  }
  for (uint32_t i = 0; i < actual_read_length; i++) {
    shell_handler(out_buffer[i]);
  }
  usbd_ep_read(ep, NULL, 0, NULL);
}

static void usbd_cdc_acm_bulk_in(uint8_t ep) {}

static void cdc_acm_printf(char *fmt, ...) {
  char print_buf[64];
  va_list ap;

  va_start(ap, fmt);
  vsnprintf(print_buf, sizeof(print_buf) - 1, fmt, ap);
  va_end(ap);
  usbd_ep_write(CDC_IN_EP, (uint8_t *)print_buf, strlen(print_buf), NULL);
}
