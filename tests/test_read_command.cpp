#include "gtest/gtest.h"
#include "../teru_dap.h"

class CommandReaderTest : public ::testing::Test {
protected:
    CommandReader reader;
};

TEST_F(CommandReaderTest, ReadDAP_CMD_INFO) {
    uint8_t bytes[] = {0x00, 0x01};    // Request the vendor id

    Action act = reader.read_command(bytes);
    ASSERT_EQ(act, Action::SendInfo);

    uint8_t info_id = reader.get_requested_dap_info_id();
    ASSERT_EQ(info_id, DAP_INFO_VENDOR_ID);
}

TEST_F(CommandReaderTest, ReadDAP_CMD_HOSTSTATUS_0) {
    uint8_t bytes[] = {0x01, 0x00, 0x01};    // Set the connect status to true

    Action act = reader.read_command(bytes);
    ASSERT_EQ(act, Action::SetConnectStatus);

    bool connect_status = reader.get_connect_status();
    ASSERT_EQ(connect_status, true);
}

TEST_F(CommandReaderTest, ReadDAP_CMD_HOSTSTATUS_1) {
    uint8_t bytes[] = {0x01, 0x01, 0x01};    // Set the connect status to true

    Action act = reader.read_command(bytes);
    ASSERT_EQ(act, Action::SetRunningStatus);

    bool running_status = reader.get_running_status();
    ASSERT_EQ(running_status, true);
}

TEST_F(CommandReaderTest, ReadDAP_CMD_CONNECT_0) {
    uint8_t bytes[] = {0x02, 0x01};    // Connect with SWD mode

    Action act = reader.read_command(bytes);
    ASSERT_EQ(act, Action::ConnectSWD);

    PortMode mode = reader.get_port_mode();
    ASSERT_EQ(mode, PortMode::SWD);
}

TEST_F(CommandReaderTest, ReadDAP_CMD_CONNECT_1) {
    uint8_t bytes[] = {0x02, 0x02};    // Connect with JTAG mode

    Action act = reader.read_command(bytes);
    ASSERT_EQ(act, Action::ConnectJTAG);

    PortMode mode = reader.get_port_mode();
    ASSERT_EQ(mode, PortMode::JTAG);
}
