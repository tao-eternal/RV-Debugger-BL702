set(BSP_COMMON_DIR ${CMAKE_SOURCE_DIR}/bsp/bsp_common)
set(TARGET_REQUIRED_LIBS usb_stack fatfs shell ${CMAKE_CURRENT_LIST_DIR}/elfloader/libelfloader.a)
set(TARGET_REQUIRED_PRIVATE_INCLUDE ${BSP_COMMON_DIR}/usb)
set(TARGET_REQUIRED_SRCS ${BSP_COMMON_DIR}/usb/usb_dc.c
                        ${BSP_COMMON_DIR}/fatfs/fatfs_usb.c
                        ${BSP_COMMON_DIR}/fatfs/fatfs_errorcode.c
                        ${CMAKE_CURRENT_LIST_DIR}/usb_msc.c)
list(APPEND GLOBAL_C_FLAGS -DFF_MAX_SS=4096)
set(mains main.c)
generate_bin()
