//
// teru-dap configuration header file
//

/* USB peripheral */
#define USE_FS_HID
//#define USE_FS_WINUSB
//#define USE_HS_HID
//#define USE_HS_WINUSB

/* Set the default mode for the connection; 1 = SWD, 2 = JTAG */
#define DAP_DEFAULT_PORT 1U

#if defined(USE_FS_HID) && (defined(USE_FS_WINUSB) || defined(USE_HS_HID) || defined(USE_HS_WINUSB))
#error "Multiple USB peripherals are specified."
#endif
#if defined(USE_FS_WINUSB) && (defined(USE_HS_HID) || defined(USE_HS_WINUSB) || defined(USB_FS_HID))
#error "Multiple USB peripherals are specified."
#endif
#if defined(USE_HS_HID) && (defined(USE_HS_WINUSB) || defined(USB_FS_HID) || defined(USE_FS_WINUSB))
#error "Multiple USB peripherals are specified."
#endif
#if defined(USE_HS_WINUSB) && (defined(USB_FS_HID) || defined(USE_FS_WINUSB) || defined(USE_HS_HID))
#error "Multiple USB peripherals are specified."
#endif
