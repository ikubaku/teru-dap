#include "teru_dap.h"

#include <cstdlib>

CommandReader::CommandReader() {
    this->dap_info = DAP_INFO_VENDOR_ID;
    this->connect_status = false;
    this->running_status = false;
#if DAP_DEFAULT_PORT == 1
    this->dap_mode = PortMode::SWD;
#elif DAP_DEFAULT_PORT == 2
    this->dap_mode = PortMode::JTAG;
#endif
    this->dap_index = 0;
    for(size_t i=0; i<sizeof(this->reg_abort); i++) {
        ((volatile uint8_t *)this->reg_abort)[i] = 0x00;
    }
}

Action CommandReader::read_command(const uint8_t * buffer) {
    uint8_t cid = buffer[0];
    switch(cid) {
        case DAP_CMD_INFO:
            return this->read_cmd_info(buffer);
        case DAP_CMD_HOST_STATUS:
            return this->read_cmd_host_status(buffer);
        case DAP_CMD_CONNECT:
            return this->read_cmd_connect(buffer);
        case DAP_CMD_DISCONNECT:
            return Action::Disconnect;
        case DAP_CMD_WRITE_ABORT:
            return this->read_cmd_write_abort(buffer);
        case DAP_CMD_DELAY:
            return Action::Undefined;
        case DAP_CMD_RESET_TARGET:
            return Action::Undefined;
        case DAP_CMD_SWJ_PINS:
            return Action::Undefined;
        case DAP_CMD_SWJ_CLOCK:
            return Action::Undefined;
        case DAP_CMD_SWJ_SEQUENCE:
            return Action::Undefined;
        case DAP_CMD_SWD_CONFIGURE:
            return Action::Undefined;
        case DAP_CMD_SWD_SEQUENCE:
            return Action::Undefined;
        case DAP_CMD_SWO_TRANSPORT:
            return Action::Undefined;
        case DAP_CMD_SWO_MODE:
            return Action::Undefined;
        case DAP_CMD_SWO_BAUDRATE:
            return Action::Undefined;
        case DAP_CMD_SWO_CONTROL:
            return Action::Undefined;
        case DAP_CMD_SWO_STATUS:
            return Action::Undefined;
        case DAP_CMD_SWO_EXTENDED_STATUS:
            return Action::Undefined;
        case DAP_CMD_SWO_DATA:
            return Action::Undefined;
        case DAP_CMD_JTAG_SEQUENCE:
            return Action::Undefined;
        case DAP_CMD_JTAG_CONFIGURE:
            return Action::Undefined;
        case DAP_CMD_JTAG_IDCODE:
            return Action::Undefined;
        case DAP_CMD_TRANSFER_CONFIGURE:
            return Action::Undefined;
        case DAP_CMD_TRANSFER:
            return Action::Undefined;
        case DAP_CMD_TRANSFER_BLOCK:
            return Action::Undefined;
        case DAP_CMD_TRANSFER_ABORT:
            return Action::Undefined;
        case DAP_CMD_EXECUTE_COMMANDS:
            return Action::Undefined;
        case DAP_CMD_QUEUE_COMMANDS:
            return Action::Undefined;
        default:
            return Action::Undefined;
    }
}

Action CommandReader::read_cmd_info(const uint8_t * buffer) {
    this->dap_info = buffer[1];
    return Action::SendInfo;
}

Action CommandReader::read_cmd_host_status(const uint8_t * buffer) {
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

Action CommandReader::read_cmd_connect(const uint8_t * buffer) {
    uint8_t mode = buffer[1];
    if(mode == DAP_MODE_DEFAULT) {
#if DAP_DEFAULT_PORT == 1
        this->dap_mode = PortMode::SWD;
        return Action::ConnectSWD;
#elif DAP_DEFAULT_PORT == 2
        this->dap_mode = PortMode::JTAG;
        return Action::ConnectJTAG;
#endif
    } else if(mode == DAP_MODE_SWD) {
        this->dap_mode = PortMode::SWD;
        return Action::ConnectSWD;
    } else if(mode == DAP_MODE_JTAG) {
        this->dap_mode = PortMode::JTAG;
        return Action::ConnectJTAG;
    } else {
        return Action::Invalid;
    }
}

Action CommandReader::read_cmd_write_abort(const uint8_t * buffer) {
    uint8_t index = buffer[1];
    this->dap_index = index;
    for(size_t i=0; i<sizeof(this->reg_abort); i++) {
        this->reg_abort[i] = buffer[2+i];
    }
    return Action::WriteABORT;
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

PortMode CommandReader::get_port_mode() {
    return this->dap_mode;
}

uint8_t CommandReader::get_dap_index() {
    return this->dap_index;
}

const uint8_t * CommandReader::get_reg_abort() {
    return (const uint8_t *)this->reg_abort;
}
