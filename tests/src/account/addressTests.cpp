#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "constants.h"

#include "account/address.h"

using namespace std;
using namespace testing;
using namespace iotex;

class AddressTests : public Test
{
};

TEST_F(AddressTests, createFromEth)
{
    iotex::Address addr(testAddressEth, AddressFormat::ETH);
    ASSERT_STREQ(testAddressEth_Prefixed, addr.Eth());
    ASSERT_STREQ(testAddressIo, addr.Io());
    ASSERT_EQ(0, memcmp(testAddressEthBytes, addr.Bytes(), ETH_ADDRESS_SIZE));
}

