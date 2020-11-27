#include "gtest/gtest.h"
#include "../teru_dap.h"

TEST(ReadCommandTest, ReadDAP_CMD_INFO) {
    const uint8_t bytes[] = {0x00, 0x01};    // Request the vendor id
    EXPECT_EQ(read_command(bytes), TERU_ACT_INFO);
    EXPECT_EQ(dap_info, DAP_INFO_VENDOR_ID);
}
