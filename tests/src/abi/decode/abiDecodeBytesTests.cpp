#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "abi/abiDecode.h"
#include "contract/contract.h"
#include "signer/signer.h"

using namespace std;
using namespace testing;
using namespace iotex;
using namespace iotex::abi::decode;

class AbiDecodeBytesTests : public Test
{
    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(AbiDecodeBytesTests, Static3)
{
    char encoded[] = "616263";
    uint8_t expected[] = {0x61, 0x62, 0x63};
    uint8_t decoded[3] = {0};
    ResultCode res = decodeStaticBytes(encoded, 3, decoded);
    ASSERT_EQ(ResultCode::SUCCESS, res);
    ASSERT_EQ(0, memcmp(expected, decoded, 3));
}

TEST_F(AbiDecodeBytesTests, Static42)
{
    char encoded[] = "010203040506070809112233445566778899102030405060708090aabbccddee11223344556677889900";
    uint8_t expected[42] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80, 0x90, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x00 };
    uint8_t decoded[42];
    ResultCode res = decodeStaticBytes(encoded, 42, decoded);
    ASSERT_EQ(ResultCode::SUCCESS, res);
    ASSERT_EQ(0, memcmp(expected, decoded, 42));
}

TEST_F(AbiDecodeBytesTests, Dynamic3)
{
    char encoded[] = 
        // Header - offset to the start of the bytes
        "0000000000000000000000000000000000000000000000000000000000000020"
        // Number of bytes
        "0000000000000000000000000000000000000000000000000000000000000003"
        // Data
        "616263";
    uint8_t expected[] = {0x61, 0x62, 0x63};
    std::vector<uint8_t> decoded;
    ResultCode res = decodeDynamicBytes(encoded, decoded);
    ASSERT_EQ(ResultCode::SUCCESS, res);
    ASSERT_EQ(3, decoded.size());
    ASSERT_EQ(0, memcmp(expected, decoded.data(), 3));
}

TEST_F(AbiDecodeBytesTests, Dynamic3_WithoutHeader)
{
    char encoded[] = 
        // Number of bytes
        "0000000000000000000000000000000000000000000000000000000000000003"
        // Data
        "616263";
    uint8_t expected[] = {0x61, 0x62, 0x63};
    std::vector<uint8_t> decoded;
    ResultCode res = decodeDynamicBytes(encoded, decoded, false);
    ASSERT_EQ(ResultCode::SUCCESS, res);
    ASSERT_EQ(3, decoded.size());
    ASSERT_EQ(0, memcmp(expected, decoded.data(), 3));
}

TEST_F(AbiDecodeBytesTests, Dynamic42)
{
    char encoded[] = 
        // Header - offset to the start of the bytes
        "0000000000000000000000000000000000000000000000000000000000000020"
        // Number of bytes
        "000000000000000000000000000000000000000000000000000000000000002A"
        // Data
        "010203040506070809112233445566778899102030405060708090aabbccddee"
        "11223344556677889900";
    uint8_t expected[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80, 0x90, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x00 };
    std::vector<uint8_t> decoded;
    ResultCode res = decodeDynamicBytes(encoded, decoded);
    ASSERT_EQ(ResultCode::SUCCESS, res);
    ASSERT_EQ(42, decoded.size());
    ASSERT_EQ(0, memcmp(expected, decoded.data(), 3));
}

TEST_F(AbiDecodeBytesTests, Dynamic_NotEnoughBytesForHeader_Fails)
{
    char encoded[] = 
        // Header - offset to the start of the bytes
        "00000000000000000000000000000000000000000000000000000000000000";;
    std::vector<uint8_t> decoded;
    ResultCode res = decodeDynamicBytes(encoded, decoded, false);
    ASSERT_EQ(ResultCode::ERROR_BAD_PARAMETER, res);
}

TEST_F(AbiDecodeBytesTests, Dynamic_NotEnoughBytesForHeaderAndSize_Fails)
{
    char encoded[] = 
        // Header - offset to the start of the bytes
        "0000000000000000000000000000000000000000000000000000000000000001";
    std::vector<uint8_t> decoded;
    ResultCode res = decodeDynamicBytes(encoded, decoded);
    ASSERT_EQ(ResultCode::ERROR_BAD_PARAMETER, res);
}

TEST_F(AbiDecodeBytesTests, Dynamic_NotEnoughBytesForData_Fails)
{
    char encoded[] = 
        // Number of bytes
        "0000000000000000000000000000000000000000000000000000000000000003"
        // Data
        "6162";
    std::vector<uint8_t> decoded;
    ResultCode res = decodeDynamicBytes(encoded, decoded);
    ASSERT_EQ(ResultCode::ERROR_BAD_PARAMETER, res);
}
