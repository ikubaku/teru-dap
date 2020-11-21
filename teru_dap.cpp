#include "teru_dap.h"

#include <iostream>

uint8_t read_command(const uint8_t * buffer) {
    uint8_t cid = buffer[0];
    switch(cid) {
        case DAP_CMD_INFO:
            dap_info = buffer[1];
            return TERU_ACT_INFO;
        case DAP_CMD_HOST_STATUS:
            return TERU_ACT_HOST_STATUS;
        case DAP_CMD_CONNECT:
            return TERU_ACT_CONNECT;
        case DAP_CMD_DISCONNECT:
            return TERU_ACT_DISCONNECT;
        case DAP_CMD_WRITE_ABORT:
            return TERU_ACT_WRITE_ABORT;
        case DAP_CMD_DELAY:
            return TERU_ACT_DELAY;
        case DAP_CMD_RESET_TARGET:
            return TERU_ACT_RESET_TARGET;
        case DAP_CMD_SWJ_PINS:
            return TERU_ACT_SWJ_PINS;
        case DAP_CMD_SWJ_CLOCK:
            return TERU_ACT_SWJ_CLOCK;
        case DAP_CMD_SWJ_SEQUENCE:
            return TERU_ACT_SWJ_SEQUENCE;
        case DAP_CMD_SWD_CONFIGURE:
            return TERU_ACT_SWD_CONFIGURE;
        case DAP_CMD_SWD_SEQUENCE:
            return TERU_ACT_SWD_SEQUENCE;
        case DAP_CMD_SWO_TRANSPORT:
            return TERU_ACT_SWO_TRANSPORT;
        case DAP_CMD_SWO_MODE:
            return TERU_ACT_SWO_MODE;
        case DAP_CMD_SWO_BAUDRATE:
            return TERU_ACT_SWO_BAUDRATE;
        case DAP_CMD_SWO_CONTROL:
            return TERU_ACT_SWO_CONTROL;
        case DAP_CMD_SWO_STATUS:
            return TERU_ACT_SWO_STATUS;
        case DAP_CMD_SWO_EXTENDED_STATUS:
            return TERU_ACT_SWO_EXTENDED_STATUS;
        case DAP_CMD_SWO_DATA:
            return TERU_ACT_SWO_DATA;
        case DAP_CMD_JTAG_SEQUENCE:
            return TERU_ACT_JTAG_SEQUENCE;
        case DAP_CMD_JTAG_CONFIGURE:
            return TERU_ACT_JTAG_CONFIGURE;
        case DAP_CMD_JTAG_IDCODE:
            return TERU_ACT_JTAG_IDCODE;
        case DAP_CMD_TRANSFER_CONFIGURE:
            return TERU_ACT_TRANSFER_CONFIGURE;
        case DAP_CMD_TRANSFER:
            return TERU_ACT_TRANSFER;
        case DAP_CMD_TRANSFER_BLOCK:
            return TERU_ACT_TRANSFER_BLOCK;
        case DAP_CMD_TRANSFER_ABORT:
            return TERU_ACT_TRANSFER_ABORT;
        case DAP_CMD_EXECUTE_COMMANDS:
            return TERU_ACT_EXECUTE_COMMANDS;
        case DAP_CMD_QUEUE_COMMANDS:
            return TERU_ACT_QUEUE_COMMANDS;
        default:
            return TERU_ACT_UNDEFINED;
    }
}
