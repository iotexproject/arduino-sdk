#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "signer/signer.h"

using namespace std;
using namespace testing;
using namespace iotex;

class SignerTests : public Test
{
    public: 
        void SetUp() override {}

        void TearDown() override {}

        char addressStr[ETH_ADDRESS_STRLEN+1];
};

// --------------------- str2hex ------------------------------

TEST_F(SignerTests, hex2Str_Skips0x_AndAcceptsAllCaps)
{
    char input[] = "0x0123456789abcdefABCDEF";
    uint8_t expected[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xAB, 0xCD, 0xEF};
    uint8_t output[sizeof(expected)] = {0};
    signer.str2hex(input, output, sizeof(output));
    ASSERT_EQ(0, memcmp(expected, output, sizeof(expected)));
}