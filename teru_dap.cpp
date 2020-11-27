#include "teru_dap.h"

CommandReader::CommandReader() {
    this->dap_info = DAP_INFO_VENDOR_ID;
    this->connect_status = false;
    this->running_status = false;
}

uint8_t CommandReader::read_command(const uint8_t * buffer) {
    uint8_t cid = buffer[0];
    switch(cid) {
        case DAP_CMD_INFO:
            this->dap_info = buffer[1];
            return TERU_ACT_INFO;
        case DAP_CMD_HOST_STATUS: {
            uint8_t type = buffer[1];
            uint8_t status = buffer[2];
            if (type == DAP_HSTATUS_CONNECT) {
                this->connect_status = status;
            } else if (type == DAP_HSTATUS_RUNNING) {
                this->running_status = status;
            } else {
                return TERU_ACT_INVALID;
            }
            return TERU_ACT_HOST_STATUS;
        }
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

uint8_t CommandReader::get_requested_dap_info_id() {
    return this->dap_info;
}

bool CommandReader::get_connect_status() {
    return this->connect_status;
}

bool CommandReader::get_running_status() {
    return this->running_status;
}
