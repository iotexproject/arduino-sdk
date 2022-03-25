#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "abi/abiDecode.h"
#include "contract/contract.h"
#include "signer/signer.h"

using namespace std;
using namespace testing;
using namespace iotex;
using namespace iotex::abi::decode;

class AbiDecodeBoolTests : public Test
{
    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(AbiDecodeBoolTests, DecodesFalse)
{
    const char encoded[] = "0000000000000000000000000000000000000000000000000000000000000000";
    bool decoded = decodeBool(encoded);
    ASSERT_EQ(false, decoded);
}

TEST_F(AbiDecodeBoolTests, DecodesTrue)
{
    const char encoded[] = "0000000000000000000000000000000000000000000000000000000000000001";
    bool decoded = decodeBool(encoded);
    ASSERT_EQ(true, decoded);
}