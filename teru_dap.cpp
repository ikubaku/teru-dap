#include "teru_dap.h"

CommandReader::CommandReader() {
    this->dap_info = DAP_INFO_VENDOR_ID;
    this->connect_status = false;
    this->running_status = false;
}

Action CommandReader::read_command(const uint8_t * buffer) {
    uint8_t cid = buffer[0];
    switch(cid) {
        case DAP_CMD_INFO:
            this->dap_info = buffer[1];
            return Action::SendInfo;
        case DAP_CMD_HOST_STATUS: {
            uint8_t type = buffer[1];
            uint8_t status = buffer[2];
            if (type == DAP_HSTATUS_CONNECT) {
                this->connect_status = status;
                return Action::SetConnectStatus;
            } else if (type == DAP_HSTATUS_RUNNING) {
                this->running_status = status;
                return Action::SetRunningStatus;
            } else {
                return Action::Invalid;
            }
        }
        case DAP_CMD_CONNECT:
            return Action::SetConnectStatus;
        case DAP_CMD_DISCONNECT:
            return Action::SetConnectStatus;
        case DAP_CMD_WRITE_ABORT:
            return Action::SetConnectStatus;
        case DAP_CMD_DELAY:
            return Action::SetConnectStatus;
        case DAP_CMD_RESET_TARGET:
            return Action::SetConnectStatus;
        case DAP_CMD_SWJ_PINS:
            return Action::SetConnectStatus;
        case DAP_CMD_SWJ_CLOCK:
            return Action::SetConnectStatus;
        case DAP_CMD_SWJ_SEQUENCE:
            return Action::SetConnectStatus;
        case DAP_CMD_SWD_CONFIGURE:
            return Action::SetConnectStatus;
        case DAP_CMD_SWD_SEQUENCE:
            return Action::SetConnectStatus;
        case DAP_CMD_SWO_TRANSPORT:
            return Action::SetConnectStatus;
        case DAP_CMD_SWO_MODE:
            return Action::SetConnectStatus;
        case DAP_CMD_SWO_BAUDRATE:
            return Action::SetConnectStatus;
        case DAP_CMD_SWO_CONTROL:
            return Action::SetConnectStatus;
        case DAP_CMD_SWO_STATUS:
            return Action::SetConnectStatus;
        case DAP_CMD_SWO_EXTENDED_STATUS:
            return Action::SetConnectStatus;
        case DAP_CMD_SWO_DATA:
            return Action::SetConnectStatus;
        case DAP_CMD_JTAG_SEQUENCE:
            return Action::SetConnectStatus;
        case DAP_CMD_JTAG_CONFIGURE:
            return Action::SetConnectStatus;
        case DAP_CMD_JTAG_IDCODE:
            return Action::SetConnectStatus;
        case DAP_CMD_TRANSFER_CONFIGURE:
            return Action::SetConnectStatus;
        case DAP_CMD_TRANSFER:
            return Action::SetConnectStatus;
        case DAP_CMD_TRANSFER_BLOCK:
            return Action::SetConnectStatus;
        case DAP_CMD_TRANSFER_ABORT:
            return Action::SetConnectStatus;
        case DAP_CMD_EXECUTE_COMMANDS:
            return Action::SetConnectStatus;
        case DAP_CMD_QUEUE_COMMANDS:
            return Action::SetConnectStatus;
        default:
            return Action::SetConnectStatus;
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
