#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "abi/abiDecode.h"
#include "contract/contract.h"
#include "signer/signer.h"

using namespace std;
using namespace testing;
using namespace iotex;
using namespace iotex::abi::decode;

class AbiDecodeAddressTests : public Test
{
    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(AbiDecodeAddressTests, DecodesCorrectly)
{
    const char encoded[] = "00000000000000000000000019e7e376e7c213b7e7e7e46cc70a5dd086daff2a";
    char decoded[ETH_ADDRESS_C_STRING_SIZE];
    ResultCode res = decodeAddress(encoded, decoded);
    ASSERT_EQ(ResultCode::SUCCESS, res);
    ASSERT_STREQ("0x19e7e376e7c213b7e7e7e46cc70a5dd086daff2a", decoded);
}
