#include "gtest/gtest.h"
#include "../teru_dap.h"

class CommandReaderTest : public ::testing::Test {
protected:
    CommandReader reader;
};

TEST_F(CommandReaderTest, ReadDAP_CMD_INFO) {
    uint8_t bytes[] = {0x00, 0x01};    // Request the vendor id

    uint8_t act = reader.read_command(bytes);
    ASSERT_EQ(act, TERU_ACT_INFO);

    uint8_t info_id = reader.get_requested_dap_info_id();
    ASSERT_EQ(info_id, DAP_INFO_VENDOR_ID);
}

TEST_F(CommandReaderTest, ReadDAP_CMD_HOSTSTATUS) {
    uint8_t bytes[] = {0x01, 0x00, 0x01};    // Set the connect status to true

    uint8_t act = reader.read_command(bytes);
    ASSERT_EQ(act, TERU_ACT_HOST_STATUS);

    bool connect_status = reader.get_connect_status();
    ASSERT_EQ(connect_status, true);
    bool running_status = reader.get_running_status();
    ASSERT_EQ(running_status, false);
}
