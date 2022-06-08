#pragma once

#include "hal_usb.h"
#include "usbd_core.h"

#define DS_OUT_EP USB_SET_EP_OUT(2)
#define DS_IN_EP USB_SET_EP_IN(6)

/* DSLogic Pro */
#define USBD_VID (0x2a0e)
#define USBD_PID (0x0003)
#define USBD_BCD (0x0001)

/* external funtion and data */
extern struct device *usb_dc_init(void);
extern USB_DESC_SECTION uint8_t usb_descriptor[];