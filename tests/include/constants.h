#pragma once

namespace 
{
    // Test server details - using the python fake server.
    // See tools/server-fake.
    const char serverHost[] = "localhost";
    const int serverPort = 10000;
    const char baseUrl[] = "";

    // Address with all ones private key used for testing.
    const char testAddressPrivateKey[] = "1111111111111111111111111111111111111111111111111111111111111111";
    const uint8_t testAddressPrivateKeyBytes[] = { 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11 };
    const char testAddressPublicKey[] = "044f355bdcb7cc0af728ef3cceb9615d90684bb5b2ca5f859ab0f0b704075871aa385b6b1b8ead809ca67454d9683fcf2ba03456d6fe2c4abe2b07f0fbdbb2f1c1";
    const char testAddressIo[] = "io1r8n7xah8cgfm0el8u3kvwzja6zrd4le2zce7sk";
    const char testAddressEth[] = "19e7e376e7c213b7e7e7e46cc70a5dd086daff2a";
    const uint8_t testAddressEthBytes[] = { 0x19, 0xe7, 0xe3, 0x76, 0xe7, 0xc2, 0x13, 0xb7, 0xe7, 0xe7, 0xe4, 0x6c, 0xc7, 0x0a, 0x5d, 0xd0, 0x86, 0xda, 0xff, 0x2a};
    
    // Another address used for tests
    const char testAddress2Eth[] = "19e7e376e7c213b7e7e7e46cc70a5dd086daff2b";
    const uint8_t testAddress2EthBytes[] = { 0x19, 0xe7, 0xe3, 0x76, 0xe7, 0xc2, 0x13, 0xb7, 0xe7, 0xe7, 0xe4, 0x6c, 0xc7, 0x0a, 0x5d, 0xd0, 0x86, 0xda, 0xff, 0x2b};

    // Other constants used in tests. 
    const char testTransactionHash[] = "e19dfb0c84799fc43217287d0d81369348279a0b3b32d0ad2f973ee5aaa392ae";
    const char testExecutionHash[] = "55b172298e80dff0fa929c7c7f7ecc266baf48e33aa226b3fd48d4de870b1efa";
    const char vitaTokenAddress[] = "io1hp6y4eqr90j7tmul4w2wa8pm7wx462hq0mg4tw";
    const char sendDataContractAddress[] = "io1n49gavyahsukdvvxxandkxephdx93n3atcrqur";

    inline void printhex(uint8_t* data, int length)
    {
        for (uint8_t i = 0; i < length; i++)
        {
            printf("%02x", data[i]);
        }
        printf("\n");
    }

    inline void printdec(uint8_t* data, int length)
    {
        for (uint8_t i = 0; i < length; i++)
        {
            printf("%u ", data[i]);
        }
        printf("\n");
    }
}