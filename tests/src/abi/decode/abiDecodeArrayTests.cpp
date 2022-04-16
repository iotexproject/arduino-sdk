#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "abi/abiDecode.h"
#include "contract/contract.h"
#include "signer/signer.h"

using namespace std;
using namespace testing;
using namespace iotex;
using namespace iotex::abi::decode;

class AbiDecodeArrayTests : public Test
{
    void SetUp() override {}

    void TearDown() override {}
};

// Static uint
TEST_F(AbiDecodeArrayTests, StaticUint8_Ok)
{
    char encoded[] = 
        "0000000000000000000000000000000000000000000000000000000000000011"
        "0000000000000000000000000000000000000000000000000000000000000064";
    uint8_t decoded[2];
    ResultCode res = decodeStaticArrayUint8(encoded, 2, decoded);
    ASSERT_EQ(ResultCode::SUCCESS, res);
    ASSERT_EQ(decoded[0], 17);
    ASSERT_EQ(decoded[1], 100);
}

TEST_F(AbiDecodeArrayTests, StaticUint16_Ok)
{
    char encoded[] = 
        "0000000000000000000000000000000000000000000000000000000000001111"
        "0000000000000000000000000000000000000000000000000000000000002364";
    uint16_t decoded[2];
    ResultCode res = decodeStaticArrayUint16(encoded, 2, decoded);
    ASSERT_EQ(ResultCode::SUCCESS, res);
    ASSERT_EQ(decoded[0], 4369);
    ASSERT_EQ(decoded[1], 9060);
}

TEST_F(AbiDecodeArrayTests, StaticUint32_Ok)
{
    char encoded[] = 
        "00000000000000000000000000000000000000000000000000000000ffffffff"
        "0000000000000000000000000000000000000000000000000000000012342364";
    uint32_t decoded[2];
    ResultCode res = decodeStaticArrayUint32(encoded, 2, decoded);
    ASSERT_EQ(ResultCode::SUCCESS, res);
    ASSERT_EQ(decoded[0], 4294967295);
    ASSERT_EQ(decoded[1], 305406820);
}

TEST_F(AbiDecodeArrayTests, StaticUint64_Ok)
{
    char encoded[] = 
        "000000000000000000000000000000000000000000000000ffffffffffffffff"
        "00000000000000000000000000000000000000000000000000000abd12342364";
    uint64_t decoded[2];
    ResultCode res = decodeStaticArrayUint64(encoded, 2, decoded);
    ASSERT_EQ(ResultCode::SUCCESS, res);
    ASSERT_EQ(decoded[0], 18446744073709551615);
    ASSERT_EQ(decoded[1], 11807170503524);
}

TEST_F(AbiDecodeArrayTests, StaticBigInt_Ok)
{
    char encoded[] = 
        "0000000000000000000000000000000000000000000000000000000000000011"
        "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff";
    iotex::bignum::Bignum decoded[2];
    ResultCode res = decodeStaticArrayUint256(encoded, 2, decoded);
    ASSERT_EQ(ResultCode::SUCCESS, res);
    ASSERT_EQ(decoded[0], bignum::Bignum("11", bignum::NumericBase::Base16));
    ASSERT_EQ(decoded[1], bignum::Bignum("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff", bignum::NumericBase::Base16));
}

// TODO int

// Static bool
TEST_F(AbiDecodeArrayTests, StaticBool_Ok)
{
    char encoded[] = 
        "0000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000001";
    bool decoded[2];
    ResultCode res = decodeStaticArrayBool(encoded, 2, decoded);
    ASSERT_EQ(ResultCode::SUCCESS, res);
    ASSERT_EQ(decoded[0], false);
    ASSERT_EQ(decoded[1], true);
}

// Static address
TEST_F(AbiDecodeArrayTests, StaticAddress_Ok)
{
    char encoded[] = 
        "00000000000000000000000019e7e376e7c213b7e7e7e46cc70a5dd086daff2a"
        "0000000000000000000000001234567890112233445566778899000102030405";
    uint8_t expected[][ETH_ADDRESS_C_STRING_SIZE] = {
        "0x19e7e376e7c213b7e7e7e46cc70a5dd086daff2a",
        "0x1234567890112233445566778899000102030405"
    };
    
    char decoded[2][ETH_ADDRESS_C_STRING_SIZE];

    ResultCode res = decodeStaticArrayAddress(encoded, 2, decoded);
    ASSERT_EQ(ResultCode::SUCCESS, res);
    ASSERT_EQ(0, memcmp(expected, decoded, 3));
}

TEST_F(AbiDecodeArrayTests, StaticAddress_NotEnoughBytes_Fails)
{
    char encoded[] = 
        "00000000000000000000000019e7e376e7c213b7e7e7e46cc70a5dd086daff2a"
        "00000000000000000000000012345678901122334455667788990001020304";

    ResultCode res = decodeStaticArrayAddress(encoded, 2, nullptr);
    ASSERT_EQ(ResultCode::ERROR_BAD_PARAMETER, res);
}

// Static string
TEST_F(AbiDecodeArrayTests, StaticString_Ok)
{
    char encoded[] =
        "0000000000000000000000000000000000000000000000000000000000000020"  // Offset for the array
        "0000000000000000000000000000000000000000000000000000000000000040"  // Offset for element 1
        "0000000000000000000000000000000000000000000000000000000000000080"  // Offset for element 2
        "0000000000000000000000000000000000000000000000000000000000000005"  // Element 1 size
        "6669727374000000000000000000000000000000000000000000000000000000"  // Element 1
        "0000000000000000000000000000000000000000000000000000000000000006"  // Element 2 size
        "7365636f6e640000000000000000000000000000000000000000000000000000"; // Element 2

    std::string decoded[2];
    ResultCode res = decodeStaticArrayString(encoded, 2, decoded);
    ASSERT_EQ(ResultCode::SUCCESS, res);
    ASSERT_EQ("first", decoded[0]);
    ASSERT_EQ("second", decoded[1]);
}

// Static dynamic bytes
TEST_F(AbiDecodeArrayTests, StaticDynamicBytes_Ok)
{
    char encoded[] =
        "0000000000000000000000000000000000000000000000000000000000000020"  // Offset for the array
        "0000000000000000000000000000000000000000000000000000000000000040"  // Offset for element 1
        "0000000000000000000000000000000000000000000000000000000000000080"  // Offset for element 2
        "0000000000000000000000000000000000000000000000000000000000000005"  // Element 1 size
        "6669727374000000000000000000000000000000000000000000000000000000"  // Element 1
        "0000000000000000000000000000000000000000000000000000000000000002"  // Element 2 size
        "0102000000000000000000000000000000000000000000000000000000000000"; // Element 2

    std::vector<std::vector<uint8_t>> expected = 
    {
        { 0x66, 0x69, 0x72, 0x73, 0x74 },
        { 0x01, 0x02}
    };

    std::vector<std::vector<uint8_t>> decoded;
    ResultCode res = decodeStaticArrayDynamicBytes(encoded, 2, decoded);
    ASSERT_EQ(ResultCode::SUCCESS, res);
    ASSERT_EQ(expected[0], decoded[0]);
    ASSERT_EQ(expected[1], decoded[1]);
    int a =1;
}

// Static static bytes
TEST_F(AbiDecodeArrayTests, StaticStaticBytes_Ok)
{
    const char encoded[] = 
        "0102030400000000000000000000000000000000000000000000000000000000"
        "4455660000000000000000000000000000000000000000000000000000000000";

    std::vector<std::vector<uint8_t>> expected = 
    {
        { 0x01, 0x02, 0x03, 0x04 },
        { 0x44, 0x55, 0x66, 0x00}
    };

    std::vector<std::vector<uint8_t>> decoded;

    ResultCode res = decodeStaticArrayStaticBytes(encoded, 2, 4, decoded);
    ASSERT_EQ(ResultCode::SUCCESS, res);
    ASSERT_EQ(expected[0], decoded[0]);
    ASSERT_EQ(expected[1], decoded[1]);
}