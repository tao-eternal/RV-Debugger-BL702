set(BSP_COMMON_DIR ${CMAKE_SOURCE_DIR}/bsp/bsp_common)
set(TARGET_REQUIRED_LIBS usb_stack)
#set(TARGET_REQUIRED_PRIVATE_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/ftdi)
set(TARGET_REQUIRED_SRCS ${BSP_COMMON_DIR}/usb/usb_dc.c
                        ${CMAKE_CURRENT_LIST_DIR}/usb_dscr.c)
set(mains main.c)
generate_bin()
