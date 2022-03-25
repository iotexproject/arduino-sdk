#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "abi/abiDecode.h"
#include "contract/contract.h"
#include "signer/signer.h"

using namespace std;
using namespace testing;
using namespace iotex;
using namespace iotex::abi::decode;

class AbiDecodeStringTests : public Test
{
    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(AbiDecodeStringTests, DecodesCorrectly)
{
    char encoded[] = 
        // Line 1(head): - offset to the encoded string parameter
        // Line 2: string length (encoded as uint)
        // Line 3: the string
        "0000000000000000000000000000000000000000000000000000000000000020"
        "0000000000000000000000000000000000000000000000000000000000000020"
        "6162636465666768696a6b6c6d6e6f707172737475767778797a414243444546";
    std::string decoded;
    ResultCode res = decodeString(encoded, sizeof(encoded), decoded);
    ASSERT_EQ(ResultCode::SUCCESS, res);
    ASSERT_STREQ("abcdefghijklmnopqrstuvwxyzABCDEF", decoded.c_str());
}

TEST_F(AbiDecodeStringTests, DecodesLongString)
{
    char encoded[] = "000000000000000000000000000000000000000000000000000000000000002000000000000000000000000000000000000000000000000000000000000000406162636465666768696a6b6c6d6e6f707172737475767778797a4142434445464748494a4b4c4d4e4f505152535455565758595a303132333435363738392a2a";
    std::string decoded;
    ResultCode res = decodeString(encoded, sizeof(encoded), decoded);
    ASSERT_EQ(ResultCode::SUCCESS, res);
    ASSERT_STREQ("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789**", decoded.c_str());
}

TEST_F(AbiDecodeStringTests, LessThan3Words_Fails)
{
    // The encoded string is missing the header
    char encoded[] =
        "000000000000000000000000000000000000000000000000000000000000000a"
        "74657374537472696e6700000000000000000000000000000000000000000000";
    std::string decoded;
    ResultCode res = decodeString(encoded, sizeof(encoded), decoded);
    ASSERT_EQ(ResultCode::ERROR_BAD_PARAMETER, res);
}

TEST_F(AbiDecodeStringTests, WrongEncodedStringSize_Fails)
{
    char encoded[] =
        // The first line indicates a string size of 33 bytes (0x21)
        "0000000000000000000000000000000000000000000000000000000000000021"
        // But we are only passing 1 word after (32 bytes)
        "6162636465666768696a6b6c6d6e6f707172737475767778797a414243444546";
    std::string decoded;
    ResultCode res = decodeString(encoded, sizeof(encoded), decoded);
    ASSERT_EQ(ResultCode::ERROR_BAD_PARAMETER, res);
}
