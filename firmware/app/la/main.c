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

#include "bflb_platform.h"
#include "led_ctrl.h"
#include "usb_dscr.h"

/* Protocol commands */
#define CMD_GET_FW_VERSION 0xb0
#define CMD_START 0xb1
#define CMD_GET_REVID_VERSION 0xb2

#define CMD_START_FLAGS_CLK_CTL2_POS 4
#define CMD_START_FLAGS_WIDE_POS 5
#define CMD_START_FLAGS_CLK_SRC_POS 6

#define CMD_START_FLAGS_CLK_CTL2 (1 << CMD_START_FLAGS_CLK_CTL2_POS)
#define CMD_START_FLAGS_SAMPLE_8BIT (0 << CMD_START_FLAGS_WIDE_POS)
#define CMD_START_FLAGS_SAMPLE_16BIT (1 << CMD_START_FLAGS_WIDE_POS)

#define CMD_START_FLAGS_CLK_30MHZ (0 << CMD_START_FLAGS_CLK_SRC_POS)
#define CMD_START_FLAGS_CLK_48MHZ (1 << CMD_START_FLAGS_CLK_SRC_POS)

#define FX2LAFW_VERSION_MAJOR 1
#define FX2LAFW_VERSION_MINOR 4
static struct version_info {
  uint8_t major;
  uint8_t minor;
} const fx2la_vi = {
    .major = FX2LAFW_VERSION_MAJOR,
    .minor = FX2LAFW_VERSION_MINOR,
};
static uint8_t REV_ID = 0x01;

struct cmd_start_acquisition {
  uint8_t flags;
  uint8_t sample_delay_h;
  uint8_t sample_delay_l;
};

static void lafw_init(void);
static void lafw_poll(void);

/* Anythingelse -> USB in */
static void usbd_cdc_acm_bulk_in(uint8_t ep) {
  uint8_t ep_idx = USB_EP_GET_IDX(ep);

  /* Check if IN ep */
  if (USB_EP_GET_DIR(ep) != USB_EP_DIR_IN) {
    return -USB_DC_EP_DIR_ERR;
  }

  uint32_t timeout = 0x0000FFFF;
  while (!USB_Is_EPx_RDY_Free(ep_idx)) {
    timeout--;
    if (!timeout) {
      LOG_E("ep%d wait free timeout\r\n", ep);
      return -USB_DC_EP_TIMEOUT_ERR;
    }
  }

  uint32_t addr = USB_BASE + 0x108 + ep_idx * 0x10;
  // memcopy_to_fifo((void *)addr, (uint8_t *)ftdi_header, 2);
  uint8_t *p = (uint8_t *)addr;
  uint8_t q[] = {0xa5};
  for (int i = 0; i < 64; i++) {
    *p = *q;
  }
  USB_Set_EPx_Rdy(ep_idx);
}

static void fx2la_notify_handler(uint8_t event, void *arg) {
  switch (event) {
    case USB_EVENT_RESET:
      // usbd_ftdi_reset();
      break;
    default:
      USBD_LOG_DBG("event 0x%x\r\n", event);
      break;
  }
}

static int fx2la_vendor_request_handler(struct usb_setup_packet *pSetup,
                                        uint8_t **data, uint32_t *len) {
  LOG_W(
      "[fx2la] request 0x%02x(type: 0x%02x), i: 0x%04x, v: 0x%04x, l: 0x "
      "%04x\r ",
      pSetup->bRequest, pSetup->bmRequestType, pSetup->wIndex, pSetup->wValue,
      pSetup->wLength);

  switch (pSetup->bRequest) {
    case CMD_START:
      break;
    case CMD_GET_FW_VERSION:
      /* Populate the buffer. */
      *data = (uint8_t *)&fx2la_vi;
      *len = sizeof(struct version_info);
      break;
    case CMD_GET_REVID_VERSION:
      *data = &REV_ID;
      *len = 1;
      break;
    default:
      return -1;
  }
  return 0;
}

static struct device *usb_fs;
static usbd_class_t fx_class;
static usbd_interface_t fx_data_intf;
static usbd_endpoint_t fx_in_ep = {.ep_addr = FX_IN_EP,
                                   .ep_cb = usbd_cdc_acm_bulk_in};

/* external funtion and data */
extern struct device *usb_dc_init(void);
extern USB_DESC_SECTION uint8_t usb_descriptor[];

int main(void) {
  /* enable debug for uart0 */
  bflb_platform_print_set(0);
  bflb_platform_init(0);
  led_gpio_init();

  LED_SET(0, 1); /* led0 on */
  LED_SET(1, 0); /* led1 off */

  /* configurate USB device */
  usbd_desc_register(usb_descriptor);
  usbd_class_register(&fx_class);
  // fixme need to use fx vender MAYBE?
  (&fx_data_intf)->vendor_handler = fx2la_vendor_request_handler;
  (&fx_data_intf)->notify_handler = fx2la_notify_handler;
  usbd_class_add_interface(&fx_class, &fx_data_intf);
  usbd_interface_add_endpoint(&fx_data_intf, &fx_in_ep);
  if (NULL != (usb_fs = usb_dc_init()))
    device_control(usb_fs, DEVICE_CTRL_SET_INT,
                   (USB_SOF_IT | USB_EP2_DATA_IN_IT));
  while (!usb_device_is_configured())
    LED_TOGGLE(0); /* led0 blink for waitting usb be ready*/

  LED_SET(0, 0); /* led0 off */
  LED_SET(1, 1); /* led1 on */

  lafw_init();
  while (1) {
    lafw_poll();
    LED_SET(0, 1);
    bflb_platform_delay_ms(500);
    LED_SET(0, 0);
    bflb_platform_delay_ms(500);
  }
  return 0;
}

static void lafw_init(void) {
  //   /* Set DYN_OUT and ENH_PKT bits, as recommended by the TRM. */
  //   REVCTL = bmNOAUTOARM | bmSKIPCOMMIT;

  //   got_sud = FALSE;
  //   vendor_command = 0;

  //   /* Renumerate. */
  //   RENUMERATE_UNCOND();

  //   SETCPUFREQ(CLK_48M);

  //   USE_USB_INTS();

  //   /* TODO: Does the order of the following lines matter? */
  //   ENABLE_SUDAV();
  //   ENABLE_EP2IBN();
  //   ENABLE_HISPEED();
  //   ENABLE_USBRESET();

  //   LED_INIT();
  //   LED_ON();

  //   /* Init timer2. */
  //   RCAP2L = -500 & 0xff;
  //   RCAP2H = (-500 & 0xff00) >> 8;
  //   T2CON = 0;
  //   ET2 = 1;
  //   TR2 = 1;

  //   /* Global (8051) interrupt enable. */
  //   EA = 1;

  //   /* Setup the endpoints. */
  //   setup_endpoints();

  //   /* Put the  into GPIF master mode and setup the GPIF. */
  //   gpif_init_la();
}

static void lafw_poll(void) {
  //   if (got_sud) {
  //     handle_setupdata();
  //     got_sud = FALSE;
  //   }

  //   if (vendor_command) {
  //     switch (vendor_command) {
  //       case CMD_START:
  //         if ((EP0CS & bmEPBUSY) != 0) break;

  //         if (EP0BCL == sizeof(struct cmd_start_acquisition)) {
  //           gpif_acquisition_prepare(
  //               (const struct cmd_start_acquisition *)EP0BUF);
  //         }

  //         /* Acknowledge the vendor command. */
  //         vendor_command = 0;
  //         break;
  //       default:
  //         /* Unimplemented command. */
  //         vendor_command = 0;
  //         break;
  //     }
  //   }

  //   gpif_poll();
}