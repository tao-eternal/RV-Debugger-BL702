#include "usb_dscr.h"

#define USB_DESCRIPITOR_LEN (9 + 7 + 7)
#define USB_CONFIG_SIZE (9 + USB_DESCRIPITOR_LEN)
#define USBD_MAX_POWER (100) /* 100/2 = 50 */
#define USBD_LANGID_STRING (0x0409)

USB_DESC_SECTION uint8_t usb_descriptor[] = {
    ///////////////////////////////////////
    /// device descriptor
    ///////////////////////////////////////
    USB_DEVICE_DESCRIPTOR_INIT(USB_2_0, 0xff, 0xff, 0xff, USBD_VID, USBD_PID,
                               USBD_BCD, 0x01),

    ///////////////////////////////////////
    /// ful-speed config descriptor
    ///////////////////////////////////////
    USB_CONFIG_DESCRIPTOR_INIT(USB_CONFIG_SIZE, 0x01, 0x01,
                               USB_CONFIG_BUS_POWERED, USBD_MAX_POWER),

    ///////////////////////////////////////
    /// interface0 descriptor
    ///////////////////////////////////////
    USB_INTERFACE_DESCRIPTOR_INIT(0x00, 0x00, 0x02,
                                  USB_DEVICE_CLASS_VEND_SPECIFIC, 0xff, 0xff,
                                  USB_STRING_LANGID_INDEX),
    USB_ENDPOINT_DESCRIPTOR_INIT(DS_OUT_EP, USB_ENDPOINT_TYPE_BULK, 0x0040,
                                 0x00),
    USB_ENDPOINT_DESCRIPTOR_INIT(DS_IN_EP, USB_ENDPOINT_TYPE_BULK, 0x0040,
                                 0x00),

    ///////////////////////////////////////
    /// string0 descriptor (LANGID)
    ///////////////////////////////////////
    USB_LANGID_INIT(USBD_LANGID_STRING),
    ///////////////////////////////////////
    /// string1 descriptor (MFC)
    ///////////////////////////////////////
    0x0E,                       /* bLength */
    USB_DESCRIPTOR_TYPE_STRING, /* bDescriptorType */
    'S', 0x00,                  /* wcChar0 */
    'I', 0x00,                  /* wcChar1 */
    'P', 0x00,                  /* wcChar2 */
    'E', 0x00,                  /* wcChar3 */
    'E', 0x00,                  /* wcChar4 */
    'D', 0x00,                  /* wcChar5 */
    ///////////////////////////////////////
    /// string2 descriptor (PRODUCT)
    ///////////////////////////////////////
    0x14,                       /* bLength */
    USB_DESCRIPTOR_TYPE_STRING, /* bDescriptorType */
    'U', 0x00,                  /* wcChar0 */
    'S', 0x00,                  /* wcChar1 */
    'B', 0x00,                  /* wcChar2 */
    ' ', 0x00,                  /* wcChar3 */
    'T', 0x00,                  /* wcChar4 */
    'O', 0x00,                  /* wcChar5 */
    ' ', 0x00,                  /* wcChar6 */
    'L', 0x00,                  /* wcChar7 */
    'A', 0x00,                  /* wcChar8 */
    ///////////////////////////////////////
    /// string3 descriptor (SERIAL)
    ///////////////////////////////////////
    0x0E,                       /* bLength */
    USB_DESCRIPTOR_TYPE_STRING, /* bDescriptorType */
    'S', 0x00,                  /* wcChar0 */
    'I', 0x00,                  /* wcChar1 */
    ' ', 0x00,                  /* wcChar2 */
    '8', 0x00,                  /* wcChar3 */
    'c', 0x00,                  /* wcChar4 */
    'h', 0x00,                  /* wcChar5 */

    ///////////////////////////////////////
    /// device qualifier descriptor
    ///////////////////////////////////////
    0x0A,                                 /* bLength */
    USB_DESCRIPTOR_TYPE_DEVICE_QUALIFIER, /* bDescriptorType */
    WBVAL(USB_2_0),                       /* bcdUSB */
    0xff, 0xff, 0xff,                     /* Class Subclass Protocol */
    0x40,                                 /* Max. EP0 packet size */
    0x00,                                 /* Number of configurations */
    0x00,                                 /* Extra reserved byte */

    0x00};