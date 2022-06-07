#pragma once

#include "hal_usb.h"
#include "usbd_core.h"

#define FX_IN_EP USB_SET_EP_IN(2)

#define USBD_VID (0x1d50)
#define USBD_PID (0x608c)
#define USBD_BCD (0x0001)


/* external funtion and data */
extern struct device *usb_dc_init(void);
extern USB_DESC_SECTION uint8_t usb_descriptor[];