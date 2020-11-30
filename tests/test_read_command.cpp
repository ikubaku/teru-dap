#include "gtest/gtest.h"
#include "../teru_dap.h"

class CommandReaderTest : public ::testing::Test {
protected:
    CommandReader reader;
};

TEST_F(CommandReaderTest, ReadDAP_CMD_INFO) {
    uint8_t bytes[] = {0x00, 0x01};    // Request the vendor id

    Action act = reader.read_command(bytes, sizeof(bytes));
    ASSERT_EQ(act, Action::SendInfo);

    uint8_t info_id = reader.get_requested_dap_info_id();
    ASSERT_EQ(info_id, DAP_INFO_VENDOR_ID);
}

TEST_F(CommandReaderTest, ReadDAP_CMD_HOSTSTATUS_0) {
    uint8_t bytes[] = {0x01, 0x00, 0x01};    // Set the connect status to true

    Action act = reader.read_command(bytes, sizeof(bytes));
    ASSERT_EQ(act, Action::SetConnectStatus);

    bool connect_status = reader.get_connect_status();
    ASSERT_EQ(connect_status, true);
}

TEST_F(CommandReaderTest, ReadDAP_CMD_HOSTSTATUS_1) {
    uint8_t bytes[] = {0x01, 0x01, 0x01};    // Set the connect status to true

    Action act = reader.read_command(bytes, sizeof(bytes));
    ASSERT_EQ(act, Action::SetRunningStatus);

    bool running_status = reader.get_running_status();
    ASSERT_EQ(running_status, true);
}

TEST_F(CommandReaderTest, ReadDAP_CMD_CONNECT_0) {
    uint8_t bytes[] = {0x02, 0x01};    // Connect with SWD mode

    Action act = reader.read_command(bytes, sizeof(bytes));
    ASSERT_EQ(act, Action::ConnectSWD);

    PortMode mode = reader.get_port_mode();
    ASSERT_EQ(mode, PortMode::SWD);
}

TEST_F(CommandReaderTest, ReadDAP_CMD_CONNECT_1) {
    uint8_t bytes[] = {0x02, 0x02};    // Connect with JTAG mode

    Action act = reader.read_command(bytes, sizeof(bytes));
    ASSERT_EQ(act, Action::ConnectJTAG);

    PortMode mode = reader.get_port_mode();
    ASSERT_EQ(mode, PortMode::JTAG);
}

TEST_F(CommandReaderTest, ReadDAP_CMD_DISCONNECT) {
    uint8_t bytes[] = {0x03};

    Action act = reader.read_command(bytes, sizeof(bytes));
    ASSERT_EQ(act, Action::Disconnect);
}

TEST_F(CommandReaderTest, ReadDAP_CMD_WRITE_ABORT) {
    uint8_t bytes[] = {0x08, 0x01, 0x10, 0x32, 0x54, 0x76};

    Action act = reader.read_command(bytes, sizeof(bytes));
    ASSERT_EQ(act, Action::WriteABORT);

    uint8_t index = reader.get_dap_index();
    ASSERT_EQ(index, 0x01);

    const uint8_t * reg_abort = reader.get_reg_abort();
    ASSERT_EQ(reg_abort[0], 0x10);
    ASSERT_EQ(reg_abort[1], 0x32);
    ASSERT_EQ(reg_abort[2], 0x54);
    ASSERT_EQ(reg_abort[3], 0x76);
}

TEST_F(CommandReaderTest, ReadDAP_CMD_DELAY) {
    uint8_t bytes[] = {0x09, 0xE8, 0x03};    // Wait for 1000us

    Action act = reader.read_command(bytes, sizeof(bytes));
    ASSERT_EQ(act, Action::Delay);

    uint16_t delay_us = reader.get_delay_us();
    ASSERT_EQ(delay_us, 1000);
}

TEST_F(CommandReaderTest, ReadDAP_CMD_RESET_TARGET) {
    uint8_t bytes[] = {0x0A};

    Action act = reader.read_command(bytes, sizeof(bytes));
    ASSERT_EQ(act, Action::ResetTarget);
}

TEST_F(CommandReaderTest, ReadDAP_CMD_SWJ_PINS) {
    uint8_t bytes[] = {0x10, 0b10101010, 0b11110000, 0xC0, 0xC6, 0x2D, 0x00};    // wait=3s

    Action act = reader.read_command(bytes, sizeof(bytes));
    ASSERT_EQ(act, Action::SWJPins);

    uint8_t pin_output = reader.get_swj_pin_output();
    ASSERT_EQ(pin_output, 0b10101010);

    uint8_t pin_select = reader.get_swj_pin_select();
    ASSERT_EQ(pin_select, 0b11110000);

    uint32_t pin_wait = reader.get_swj_pin_wait();
    ASSERT_EQ(pin_wait, 3000000);
}

TEST_F(CommandReaderTest, ReadDAP_CMD_SWJ_CLOCK) {
    uint8_t bytes[] = {0x11, 0x80, 0x1A, 0x06, 0x00};    // 400kHz

    Action act = reader.read_command(bytes, sizeof(bytes));
    ASSERT_EQ(act, Action::SWJClock);

    uint32_t swj_clock = reader.get_swj_clock();
    ASSERT_EQ(swj_clock, 400000);
}

TEST_F(CommandReaderTest, ReadDAP_CMD_SWJ_SEQUENCE) {
    uint8_t bytes[] = {0x12, 0x88, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x9E, 0xE7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00};    // JTAG to SWD sequence

    Action act = reader.read_command(bytes, sizeof(bytes));
    ASSERT_EQ(act, Action::SWJSequence);

    uint8_t seq_len = reader.get_swj_sequence_len();
    ASSERT_EQ(seq_len, 136);

    const uint8_t * seq = reader.get_swj_sequence();
    for(size_t i=0; i<17; i++) {
        ASSERT_EQ(seq[i], bytes[2+i]);
    }
}
