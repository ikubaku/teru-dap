#include "teru_dap.h"

#include <cstdlib>
#include <cstring>

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
    this->delay_us = 0;
    this->swj_pins = 0b00000000;
    this->swj_select = 0b00000000;
    this->swj_pin_wait = 0;
    this->swj_clock_hz = 0;
    this->swj_sequence_len = 0;
    for(size_t i=0; i<sizeof(this->swj_sequence); i++) {
        ((volatile uint8_t *)this->swj_sequence)[i] = 0x00;
    }
}

Action CommandReader::read_command(const uint8_t * buffer, size_t len) {
    if(len < TO_U8_LENGTH(SIZE_BYTE)) {
        return Action::Invalid;
    }

    uint8_t cid = buffer[0];
    switch(cid) {
        case DAP_CMD_INFO:
            return this->read_cmd_info(buffer, len);
        case DAP_CMD_HOST_STATUS:
            return this->read_cmd_host_status(buffer, len);
        case DAP_CMD_CONNECT:
            return this->read_cmd_connect(buffer, len);
        case DAP_CMD_DISCONNECT:
            return Action::Disconnect;
        case DAP_CMD_WRITE_ABORT:
            return this->read_cmd_write_abort(buffer, len);
        case DAP_CMD_DELAY:
            return this->read_cmd_delay(buffer, len);
        case DAP_CMD_RESET_TARGET:
            return Action::ResetTarget;
        case DAP_CMD_SWJ_PINS:
            return this->read_cmd_swj_pins(buffer, len);
        case DAP_CMD_SWJ_CLOCK:
            return this->read_cmd_swj_clock(buffer, len);
        case DAP_CMD_SWJ_SEQUENCE:
            return this->read_cmd_swj_sequence(buffer, len);
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

Action CommandReader::read_cmd_info(const uint8_t * buffer, size_t len) {
    if(len != TO_U8_LENGTH(SIZE_BYTE * 2)) {
        return Action::Invalid;
    }

    this->dap_info = buffer[1];
    return Action::SendInfo;
}

Action CommandReader::read_cmd_host_status(const uint8_t * buffer, size_t len) {
    if(len != TO_U8_LENGTH(SIZE_BYTE * 3)) {
        return Action::Invalid;
    }

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

Action CommandReader::read_cmd_connect(const uint8_t * buffer, size_t len) {
    if(len != TO_U8_LENGTH(SIZE_BYTE * 2)) {
        return Action::Invalid;
    }

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

Action CommandReader::read_cmd_write_abort(const uint8_t * buffer, size_t len) {
    if(len != TO_U8_LENGTH(SIZE_BYTE * 2 + SIZE_WORD)) {
        return Action::Invalid;
    }

    uint8_t index = buffer[1];
    this->dap_index = index;
    for(size_t i=0; i<sizeof(this->reg_abort); i++) {
        this->reg_abort[i] = buffer[2+i];
    }
    return Action::WriteABORT;
}

Action CommandReader::read_cmd_delay(const uint8_t * buffer, size_t len) {
    if(len != TO_U8_LENGTH(SIZE_BYTE + SIZE_SHORT)) {
        return Action::Invalid;
    }

    this->delay_us = buffer[2] * 0x100 + buffer[1];
    return Action::Delay;
}

Action CommandReader::read_cmd_swj_pins(const uint8_t * buffer, size_t len) {
    if(len != TO_U8_LENGTH(SIZE_BYTE * 3 + SIZE_WORD)) {
        return Action::Invalid;
    }

    uint32_t pin_wait_tmp = buffer[6] * 0x1000000 + buffer[5] * 0x10000 + buffer[4] * 0x100 + buffer[3];
    if(pin_wait_tmp > 3000000) {
        return Action::Invalid;
    }

    this->swj_pins = buffer[1];
    this->swj_select = buffer[2];
    this->swj_pin_wait = pin_wait_tmp;
    return Action::SWJPins;
}

Action CommandReader::read_cmd_swj_clock(const uint8_t * buffer, size_t len) {
    if(len != TO_U8_LENGTH(SIZE_BYTE + SIZE_WORD)) {
        return Action::Invalid;
    }

    this->swj_clock_hz = buffer[4] * 0x1000000 + buffer[3] * 0x10000 + buffer[2] * 0x100 + buffer[1];
    return Action::SWJClock;
}

Action CommandReader::read_cmd_swj_sequence(const uint8_t * buffer, size_t len) {
    if(len < TO_U8_LENGTH(SIZE_BYTE * 3)) {
        return Action::Invalid;
    }

    /* Compute the copy region length */
    uint8_t seq_len_tmp = buffer[1];
    size_t seq_byte_len;
    if(seq_len_tmp == 0) {
        seq_byte_len = 32;
    } else {
        /* seq_byte_len = ceil(seq_len_tmp / 8); */
        seq_byte_len = (seq_len_tmp / 8) + ((seq_len_tmp % 8) ? 1 : 0);
    }
    if(len != TO_U8_LENGTH(SIZE_BYTE * 2) + seq_byte_len) {
        return Action::Invalid;
    }

    this->swj_sequence_len = seq_len_tmp;
    memcpy(this->swj_sequence, buffer + 2, seq_byte_len);
    return Action::SWJSequence;
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

uint16_t CommandReader::get_delay_us() {
    return this->delay_us;
}

uint8_t CommandReader::get_swj_pin_output() {
    return this->swj_pins;
}

uint8_t CommandReader::get_swj_pin_select() {
    return this->swj_select;
}

uint32_t CommandReader::get_swj_pin_wait() {
    return this->swj_pin_wait;
}

uint32_t CommandReader::get_swj_clock() {
    return this->swj_clock_hz;
}

uint8_t CommandReader::get_swj_sequence_len() {
    return this->swj_sequence_len;
}

const uint8_t * CommandReader::get_swj_sequence() {
    return (const uint8_t *)this->swj_sequence;
}
