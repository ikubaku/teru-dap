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
