#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "abi/abiDecode.h"
#include "contract/contract.h"
#include "signer/signer.h"
#include "bignum/bignum.h"

using namespace std;
using namespace testing;
using namespace iotex;
using namespace iotex::abi::decode;
using namespace iotex::bignum;

class AbiDecodeUintTests : public Test
{
    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(AbiDecodeUintTests, Uint8)
{
    // NOTE: The 2nd LSB is 0xFF. This also tests that this byte value is not converted. Because we are passing asize of 1
    char encoded[] = "000000000000000000000000000000000000000000000000000000000000ff1d";
    uint64_t decoded = 0;
    decodeUint<1>(encoded, strlen(encoded), &decoded);
    ASSERT_EQ(29, decoded);
    decoded = decodeUint8(encoded);
    ASSERT_EQ(29, decoded);
}

TEST_F(AbiDecodeUintTests, Uint16)
{
    // NOTE: The 3nd LSB is 0xFF. This also tests that this byte value is not converted. Because we are passing asize of 2
    char encoded[] = "0000000000000000000000000000000000000000000000000000000000ff040d";
    uint64_t decoded = 0;
    decodeUint<2>(encoded, strlen(encoded), &decoded);
    ASSERT_EQ(1037, decoded);
    decoded = decodeUint16(encoded);
    ASSERT_EQ(1037, decoded);
}

TEST_F(AbiDecodeUintTests, Uint24)
{                   
    // NOTE: The 4th LSB is 0xFF. This also tests that this byte value is not converted. Because we are passing asize of 3 
    char encoded[] = "00000000000000000000000000000000000000000000000000000000ff13040d";
    uint64_t decoded = 0;
    decodeUint<3>(encoded, strlen(encoded), &decoded);
    ASSERT_EQ(1246221, decoded);
}

TEST_F(AbiDecodeUintTests, Uint32)
{
    // NOTE: The 5th LSB is 0xFF. This also tests that this byte value is not converted. Because we are passing asize of 5
    char encoded[] = "000000000000000000000000000000000000000000000000000000ff1103040d";
    uint64_t decoded = 0;
    decodeUint<4>(encoded, strlen(encoded), &decoded);
    ASSERT_EQ(285410317, decoded);
    decoded = decodeUint32(encoded);
    ASSERT_EQ(285410317, decoded);
}

TEST_F(AbiDecodeUintTests, Uint64)
{
    // NOTE: The 9th LSB is 0xFF. This also tests that this byte value is not converted. Because we are passing asize of 8
    char encoded[] = "0000000000000000000000000000000000000000000000ff1122334455667788";
    uint64_t decoded = 0;
    decodeUint<8>(encoded, strlen(encoded), &decoded);
    ASSERT_EQ(1234605616436508552, decoded);
    decoded = decodeUint64(encoded);
    ASSERT_EQ(1234605616436508552, decoded);
}

TEST_F(AbiDecodeUintTests, AcceptsUppercase)
{
    char encoded[] = "000000000000000000000000000000000000000000000000000000000000ff1D";
    uint64_t decoded = 0;
    decodeUint<1>(encoded, strlen(encoded), &decoded);
    ASSERT_EQ(29, decoded);
}

TEST_F(AbiDecodeUintTests, ReturnsSuccessIfOk)
{
    char encoded[] = "000000000000000000000000000000000000000000000000000000000000ff1d";
    uint64_t decoded = 0;
    ResultCode result = decodeUint<1>(encoded, strlen(encoded), &decoded);
    ASSERT_EQ(ResultCode::SUCCESS, result);
}

TEST_F(AbiDecodeUintTests, ReturnsErrorIfInvalidHexNumber)
{
    char encoded[] = "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffgg";
    uint64_t decoded = 0;
    ResultCode result = decodeUint<1>(encoded, strlen(encoded), &decoded);
    ASSERT_EQ(ResultCode::ERROR_BAD_PARAMETER, result);
}

TEST_F(AbiDecodeUintTests, ReturnsErrorIfSizeOver8Bytes)
{
    char encoded[] = "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff";
    uint64_t decoded = 0;
    ResultCode result = decodeUint<9>(encoded, strlen(encoded), &decoded);
    ASSERT_EQ(ResultCode::ERROR_BAD_PARAMETER, result);
}

TEST_F(AbiDecodeUintTests, Handles0xPrefix)
{
    char encoded[] = "0x000000000000000000000000000000000000000000000000000000000000ff1d";
    uint64_t decoded = 0;
    ResultCode result = decodeUint<1>(encoded, strlen(encoded), &decoded);
    ASSERT_EQ(ResultCode::SUCCESS, result);
    ASSERT_EQ(29, decoded);
}

TEST_F(AbiDecodeUintTests, Bignum_Ok)
{
    const char expectedDecimal[] = "446371678961165142885801714189622662489706559239886995455";
    char encoded[] = "0000000000000000123456789acbdeffffffffffffffffffffffffffffffffff";
    
    Bignum decoded;

    // Decode as uint64
    ResultCode result = decodeBigUint(encoded, 256, decoded);
    ASSERT_EQ(ResultCode::SUCCESS, result);
    auto decodedStr = decoded.ToString(NumericBase::Base10).c_str();
    ASSERT_STREQ(expectedDecimal, decodedStr);

    // Decode as 24 bytes. This tests it handles the padding bytes correctly.
    result = decodeBigUint(encoded, 192, decoded);
    ASSERT_EQ(ResultCode::SUCCESS, result);
    decodedStr = decoded.ToString(NumericBase::Base10).c_str();
    ASSERT_STREQ(expectedDecimal, decodedStr);
}
