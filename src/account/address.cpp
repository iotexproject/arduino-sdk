#include "account/address.h"

#include "string.h"
#include <vector>
#include <string>
#include "signer/signer.h"
#include "encoder/encoder.h"

using namespace iotex;

iotex::Address::Address(const char* address, AddressFormat format)
{
    memset(_io, 0, sizeof(_io));
    memset(_eth, 0, sizeof(_eth));
    memset(_bytes, 0, sizeof(_bytes));

    if (format == AddressFormat::IO)
    {
        // TODO create from io format
    }
    else
    {
        ConstructFromEth(address);
    }
}

const char* iotex::Address::Io()
{
    return _io;
};

const char* iotex::Address::Eth()
{
    return _eth;
};

const uint8_t* iotex::Address::Bytes()
{
    return _bytes;
};

void iotex::Address::ConstructFromEth(const char* address)
{
    // Set bytes
    signer.str2hex(address, _bytes, 20);

    // Copy 0x address
    if(address[0] == '0' && address[1] == 'x')
    {
        memcpy(_eth, address, ETH_ADDRESS_C_STRING_SIZE);
    }
    else
    {
        _eth[0] = '0';
        _eth[1] = 'x';
        memcpy(_eth+2, address, ETH_ADDRESS_C_STRING_SIZE_NON_PREFIXED);
    }

    // Create io address
    std::vector<uint8_t> bytes(_bytes, _bytes + ETH_ADDRESS_SIZE);
    IotexString io = _io;
    encoder.bech32_encode(bytes, io);
    memcpy(_io, io.c_str(), IOTEX_ADDRESS_C_STRING_SIZE);
}