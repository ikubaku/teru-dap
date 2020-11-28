#ifndef TERU_DAP_TERU_DAP_H
#define TERU_DAP_TERU_DAP_H

#include <cstdint>
#include <cstdlib>
#include "config.h"

/* Maximum packet size */
#ifdef USE_FS_HID
#define DAP_PACKET_SIZE 64U
#endif
#ifdef USE_FS_WINUSB
#define DAP_PACKET_SIZE 64U
#endif
#ifdef USE_HS_HID
#define DAP_PACKET_SIZE 1024U
#endif
#ifdef USE_HS_WINUSB
#define DAP_PACKET_SIZE 512U
#endif
//#define DAP_PACKET_SIZE 512U

/* Data sizes */
#define SIZE_BYTE sizeof(uint8_t)
#define SIZE_SHORT sizeof(uint16_t)
#define SIZE_WORD sizeof(uint32_t)
#define SIZE_LWORD sizeof(uint64_t)
#define SIZE_FLOAT sizeof(uint32_t)
#define TO_U8_LENGTH(s) ((s) / sizeof(uint8_t))

/* DAP command IDs */
#define DAP_CMD_INFO 0x00
#define DAP_CMD_HOST_STATUS 0x01
#define DAP_CMD_CONNECT 0x02
#define DAP_CMD_DISCONNECT 0x03
#define DAP_CMD_WRITE_ABORT 0x08
#define DAP_CMD_DELAY 0x09
#define DAP_CMD_RESET_TARGET 0x0A

#define DAP_CMD_SWJ_PINS 0x10
#define DAP_CMD_SWJ_CLOCK 0x11
#define DAP_CMD_SWJ_SEQUENCE 0x12

#define DAP_CMD_SWD_CONFIGURE 0x13
#define DAP_CMD_SWD_SEQUENCE 0x1D

#define DAP_CMD_SWO_TRANSPORT 0x17
#define DAP_CMD_SWO_MODE 0x18
#define DAP_CMD_SWO_BAUDRATE 0x19
#define DAP_CMD_SWO_CONTROL 0x1A
#define DAP_CMD_SWO_STATUS 0x1B
#define DAP_CMD_SWO_EXTENDED_STATUS 0x1E
#define DAP_CMD_SWO_DATA 0x1C

#define DAP_CMD_JTAG_SEQUENCE 0x14
#define DAP_CMD_JTAG_CONFIGURE 0x15
#define DAP_CMD_JTAG_IDCODE 0x16

#define DAP_CMD_TRANSFER_CONFIGURE 0x04
#define DAP_CMD_TRANSFER 0x05
#define DAP_CMD_TRANSFER_BLOCK 0x06
#define DAP_CMD_TRANSFER_ABORT 0x07

#define DAP_CMD_EXECUTE_COMMANDS 0x7F
#define DAP_CMD_QUEUE_COMMANDS 0x7E

#define DAP_CMD_UNDEFINED 0xFF

/* Debug unit information kinds */
#define DAP_INFO_VENDOR_ID 0x01
#define DAP_INFO_PRODUCT_ID 0x02
#define DAP_INFO_SERIAL_NUMBER 0x03
#define DAP_INFO_CMSIS_DAP_FIRMWARE_VERSION 0x04
#define DAP_INFO_TARGET_DEVICE_VENDOR 0x05
#define DAP_INFO_TARGET_DEVICE_NAME 0x06
#define DAP_INFO_CAPABILITIES 0xF0
#define DAP_INFO_TEST_DOMAIN_TIMER 0xF1
#define DAP_INFO_SWO_TRACE_BUFFER_SIZE 0xFD
#define DAP_INFO_SWO_PACKET_COUNT 0xFE
#define DAP_INFO_PACKET_COUNT 0xFF

/* Host status types */
#define DAP_HSTATUS_CONNECT 0x00
#define DAP_HSTATUS_RUNNING 0x01

/* DAP port modes */
#define DAP_MODE_DEFAULT 0x00
#define DAP_MODE_SWD 0x01
#define DAP_MODE_JTAG 0x02

/* Response status */
#define DAP_DAP_OK 0x00
#define DAP_DAP_ERROR 0xFF

/* DAP actions */
enum Action {
    SendInfo,
    SetConnectStatus,
    SetRunningStatus,
    ConnectSWD,
    ConnectJTAG,
    Disconnect,
    WriteABORT,
    Delay,
    Invalid,
    NoOperation,
    Undefined,
};

/* DAP port modes(for the state) */
enum PortMode {
    SWD,
    JTAG,
};

#define TERU_ACT_INFO 0x00
#define TERU_ACT_HOST_STATUS 0x01
#define TERU_ACT_CONNECT 0x02
#define TERU_ACT_DISCONNECT 0x03
#define TERU_ACT_WRITE_ABORT 0x08
#define TERU_ACT_DELAY 0x09
#define TERU_ACT_RESET_TARGET 0x0A
#define TERU_ACT_SWJ_PINS 0x10
#define TERU_ACT_SWJ_CLOCK 0x11
#define TERU_ACT_SWJ_SEQUENCE 0x12
#define TERU_ACT_SWD_CONFIGURE 0x13
#define TERU_ACT_SWD_SEQUENCE 0x1D
#define TERU_ACT_SWO_TRANSPORT 0x17
#define TERU_ACT_SWO_MODE 0x18
#define TERU_ACT_SWO_BAUDRATE 0x19
#define TERU_ACT_SWO_CONTROL 0x1A
#define TERU_ACT_SWO_STATUS 0x1B
#define TERU_ACT_SWO_EXTENDED_STATUS 0x1E
#define TERU_ACT_SWO_DATA 0x1C
#define TERU_ACT_JTAG_SEQUENCE 0x14
#define TERU_ACT_JTAG_CONFIGURE 0x15
#define TERU_ACT_JTAG_IDCODE 0x16
#define TERU_ACT_TRANSFER_CONFIGURE 0x04
#define TERU_ACT_TRANSFER 0x05
#define TERU_ACT_TRANSFER_BLOCK 0x06
#define TERU_ACT_TRANSFER_ABORT 0x07
#define TERU_ACT_EXECUTE_COMMANDS 0x7F
#define TERU_ACT_QUEUE_COMMANDS 0x7E
#define TERU_ACT_INVALID 0xFD
#define TERU_ACT_NOP 0xFE
#define TERU_ACT_UNDEFINED 0xFF


class CommandReader {
private:
    uint8_t dap_info;
    bool connect_status;
    bool running_status;
    PortMode dap_mode;
    uint8_t dap_index;
    uint8_t reg_abort[TO_U8_LENGTH(SIZE_WORD)];
    uint16_t delay_us;

    Action read_cmd_info(const uint8_t *, size_t);
    Action read_cmd_host_status(const uint8_t *, size_t);
    Action read_cmd_connect(const uint8_t *, size_t);
    Action read_cmd_write_abort(const uint8_t *, size_t);
    Action read_cmd_delay(const uint8_t *, size_t);

public:
    CommandReader();
    Action read_command(const uint8_t *, size_t);
    uint8_t get_requested_dap_info_id();
    bool get_connect_status();
    bool get_running_status();
    PortMode get_port_mode();
    uint8_t get_dap_index();
    const uint8_t * get_reg_abort();
    uint16_t get_delay_us();
};

#endif //TERU_DAP_TERU_DAP_H
