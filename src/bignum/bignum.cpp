#include "bignum/bignum.h"

using namespace iotex::bignum;

Bignum::Bignum()
{
    _u256 = uint256_0;
}

Bignum::Bignum(const char* str, NumericBase base)
{   
    uint8_t baseInt = 16;
    if (base == NumericBase::Base10) { baseInt = 10; }
   
    _u256 = uint256_t(str, baseInt);
}

IotexString Bignum::ToString(NumericBase base) const
{
    if (base == NumericBase::Base10)
    {
        return _u256.str(10);
    }
    else if (base == NumericBase::Base16)
    {
        return _u256.str(16);
    }
    else return "";
}

Bignum& Bignum::operator=(const Bignum& bignum)
{
    _u256 = uint256_t(bignum.ToString(NumericBase::Base16), 16);
    return *this;
}