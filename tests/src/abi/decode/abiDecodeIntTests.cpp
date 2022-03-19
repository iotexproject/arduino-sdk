#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "abi/abiDecode.h"
#include "contract/contract.h"
#include "signer/signer.h"

using namespace std;
using namespace testing;
using namespace iotex;
using namespace iotex::abi::decode;

class AbiDecodeIntTests : public Test
{
    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(AbiDecodeIntTests, Int8)
{
    char encoded[] = "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffef";
    int64_t decoded = 0;
    decodeInt<1>(encoded, strlen(encoded), &decoded);
    ASSERT_EQ(-17, decoded);
    decoded = decodeInt8(encoded);
    ASSERT_EQ(-17, decoded);
}

TEST_F(AbiDecodeIntTests, Int16)
{
    char encoded[] = "fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffec7";
    int64_t decoded = 0;
    decodeInt<2>(encoded, strlen(encoded), &decoded);
    ASSERT_EQ(-313, decoded);
    decoded = decodeInt16(encoded);
    ASSERT_EQ(-313, decoded);
}

TEST_F(AbiDecodeIntTests, Int24)
{                   
    char encoded[] = "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff85ee0";
    int64_t decoded = 0;
    decodeInt<3>(encoded, strlen(encoded), &decoded);
    ASSERT_EQ(-500000, decoded);
}

TEST_F(AbiDecodeIntTests, Int32)
{
    char encoded[] = "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffb66b0660";
    int64_t decoded = 0;
    decodeInt<4>(encoded, strlen(encoded), &decoded);
    ASSERT_EQ(-1234500000, decoded);
    decoded = decodeInt32(encoded);
    ASSERT_EQ(-1234500000, decoded);
}

TEST_F(AbiDecodeIntTests, Int64)
{
    char encoded[] = "ffffffffffffffffffffffffffffffffffffffffffffffffeede2cca68c7bff3";
    int64_t decoded = 0;
    decodeInt<8>(encoded, strlen(encoded), &decoded);
    ASSERT_EQ(-1234500000000000013, decoded);
    decoded = decodeInt64(encoded);
    ASSERT_EQ(-1234500000000000013, decoded);
}

TEST_F(AbiDecodeIntTests, AcceptsUppercase)
{
    char encoded[] = "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffEF";
    int64_t decoded = 0;
    decodeInt<1>(encoded, strlen(encoded), &decoded);
    ASSERT_EQ(-17, decoded);
}

TEST_F(AbiDecodeIntTests, ReturnsSuccessIfOk)
{
    char encoded[] = "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffef";
    int64_t decoded = 0;
    ResultCode result = decodeInt<1>(encoded, strlen(encoded), &decoded);
    ASSERT_EQ(ResultCode::SUCCESS, result);
}

TEST_F(AbiDecodeIntTests, ReturnsErrorIfInvalidHexNumber)
{
    char encoded[] = "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffgg";
    int64_t decoded = 0;
    ResultCode result = decodeInt<1>(encoded, strlen(encoded), &decoded);
    ASSERT_EQ(ResultCode::ERROR_BAD_PARAMETER, result);
}

TEST_F(AbiDecodeIntTests, ReturnsErrorIfSizeOver8Bytes)
{
    char encoded[] = "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff";
    int64_t decoded = 0;
    ResultCode result = decodeInt<9>(encoded, strlen(encoded), &decoded);
    ASSERT_EQ(ResultCode::ERROR_BAD_PARAMETER, result);
}

TEST_F(AbiDecodeIntTests, Handles0xPrefix)
{
    char encoded[] = "0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffef";
    int64_t decoded = 0;
    ResultCode result = decodeInt<1>(encoded, strlen(encoded), &decoded);
    ASSERT_EQ(ResultCode::SUCCESS, result);
    ASSERT_EQ(-17, decoded);
}