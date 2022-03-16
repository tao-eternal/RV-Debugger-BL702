#include "main.h"
#include "usbd_cdc.h"
#include "usbd_msc.h"

#define USBD_VID 0xFFFF
#define USBD_PID 0xFFFF
#define USBD_MAX_POWER 100
#define USBD_LANGID_STRING 1033

#define USB_CONFIG_SIZE (9 + CDC_ACM_DESCRIPTOR_LEN + 8 + MSC_DESCRIPTOR_LEN)

/*The following parameters need to be modified simultaneously in
 * bsp/bsp_common/fatfs/fatfs_usb.c!!*/
#define BLOCK_SIZE 4096
#define BLOCK_COUNT 64
#define FLASH_ADDR 0x00040000 /*addr start from 256k */
void usbd_msc_get_cap(uint8_t lun, uint32_t *block_num, uint16_t *block_size) {
  *block_num = BLOCK_COUNT;
  *block_size = BLOCK_SIZE;
}
int usbd_msc_sector_read(uint32_t sector, uint8_t *buffer, uint32_t length) {
  flash_read(FLASH_ADDR + sector * BLOCK_SIZE, buffer, length);
  return 0;
}
int usbd_msc_sector_write(uint32_t sector, uint8_t *buffer, uint32_t length) {
  flash_erase(FLASH_ADDR + sector * BLOCK_SIZE, BLOCK_SIZE);
  flash_write(FLASH_ADDR + sector * BLOCK_SIZE, buffer, length);
  return 0;
}

USB_DESC_SECTION const uint8_t cdc_msc_descriptor[] = {
    USB_DEVICE_DESCRIPTOR_INIT(USB_2_0, 0xEF, 0x02, 0x01, USBD_VID, USBD_PID,
                               0x0100, 0x01),
    USB_CONFIG_DESCRIPTOR_INIT(USB_CONFIG_SIZE, 0x03, 0x01,
                               USB_CONFIG_BUS_POWERED, USBD_MAX_POWER),
    CDC_ACM_DESCRIPTOR_INIT(0x00, CDC_INT_EP, CDC_OUT_EP, CDC_IN_EP, 0x02),
    ///////////////////////////////////////
    /// interface association descriptor
    ///////////////////////////////////////
    0x08,                                      /* bLength */
    USB_DESCRIPTOR_TYPE_INTERFACE_ASSOCIATION, /* bDescriptorType */
    0x02,                                      /* bFirstInterface */
    0x01,                                      /* bInterfaceCount */
    0x08,                                      /* bFunctionClass */
    0x06,                                      /* bFunctionSubClass */
    0x50,                                      /* bFunctionProtocol */
    0x00,                                      /* iFunction */

    MSC_DESCRIPTOR_INIT(0x02, MSC_OUT_EP, MSC_IN_EP, 0x00),
    ///////////////////////////////////////
    /// string0 descriptor
    ///////////////////////////////////////
    USB_LANGID_INIT(USBD_LANGID_STRING),
    ///////////////////////////////////////
    /// string1 descriptor
    ///////////////////////////////////////
    0x12,                       /* bLength */
    USB_DESCRIPTOR_TYPE_STRING, /* bDescriptorType */
    'B', 0x00,                  /* wcChar0 */
    'o', 0x00,                  /* wcChar1 */
    'u', 0x00,                  /* wcChar2 */
    'f', 0x00,                  /* wcChar3 */
    'f', 0x00,                  /* wcChar4 */
    'a', 0x00,                  /* wcChar5 */
    'l', 0x00,                  /* wcChar6 */
    'o', 0x00,                  /* wcChar7 */
    ///////////////////////////////////////
    /// string2 descriptor
    ///////////////////////////////////////
    0x22,                       /* bLength */
    USB_DESCRIPTOR_TYPE_STRING, /* bDescriptorType */
    'B', 0x00,                  /* wcChar0 */
    'o', 0x00,                  /* wcChar1 */
    'u', 0x00,                  /* wcChar2 */
    'f', 0x00,                  /* wcChar3 */
    'f', 0x00,                  /* wcChar4 */
    'a', 0x00,                  /* wcChar5 */
    'l', 0x00,                  /* wcChar6 */
    'o', 0x00,                  /* wcChar7 */
    ' ', 0x00,                  /* wcChar8 */
    'C', 0x00,                  /* wcChar9 */
    'D', 0x00,                  /* wcChar10 */
    'C', 0x00,                  /* wcChar11 */
    '_', 0x00,                  /* wcChar12 */
    'M', 0x00,                  /* wcChar14 */
    'S', 0x00,                  /* wcChar15 */
    'C', 0x00,                  /* wcChar16 */
    ///////////////////////////////////////
    /// string3 descriptor
    ///////////////////////////////////////
    0x16,                       /* bLength */
    USB_DESCRIPTOR_TYPE_STRING, /* bDescriptorType */
    'b', 0x00,                  /* wcChar0 */
    'l', 0x00,                  /* wcChar1 */
    '7', 0x00,                  /* wcChar2 */
    '0', 0x00,                  /* wcChar3 */
    '2', 0x00,                  /* wcChar4 */
    '1', 0x00,                  /* wcChar5 */
    '2', 0x00,                  /* wcChar6 */
    '3', 0x00,                  /* wcChar7 */
    '4', 0x00,                  /* wcChar8 */
    '5', 0x00,                  /* wcChar9 */
#ifdef CONFIG_USB_HS
    ///////////////////////////////////////
    /// device qualifier descriptor
    ///////////////////////////////////////
    0x0a, USB_DESCRIPTOR_TYPE_DEVICE_QUALIFIER, 0x00, 0x02, 0x02, 0x02, 0x01,
    0x40, 0x01, 0x00,
#endif
    0x00};
