#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "abi/abi.h"
#include "contract/contract.h"
#include "signer/signer.h"

using namespace std;
using namespace testing;
using namespace iotex;
using namespace iotex::abi;

class ContractTests : public Test
{
    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(ContractTests, generateBytesForUint)
{
    uint8_t buf[32] = {0};
    char strBuf[65] = {0};
    // 8 bit
    uint8_t value8 = 0xF0;
    int32_t ret = Contract::generateBytesForUint(&value8, 1, buf);
    signer.hex2str(buf, sizeof(buf), strBuf, sizeof(strBuf));
    ASSERT_STREQ("00000000000000000000000000000000000000000000000000000000000000f0", strBuf);
    // 16 bit
    uint16_t value16 = 0x1234;
    ret = Contract::generateBytesForUint((uint8_t *)&value16, 2, buf);
    signer.hex2str(buf, sizeof(buf), strBuf, sizeof(strBuf));
    ASSERT_STREQ("0000000000000000000000000000000000000000000000000000000000001234", strBuf);
    // 32 bit
    uint32_t value32 = 0x12345678;
    ret = Contract::generateBytesForUint((uint8_t *)&value32, 4, buf);
    signer.hex2str(buf, sizeof(buf), strBuf, sizeof(strBuf));
    ASSERT_STREQ("0000000000000000000000000000000000000000000000000000000012345678", strBuf);
    // 64 bit
    uint64_t value64 = 2459565876494606882;
    ret = Contract::generateBytesForUint((uint8_t *)&value64, 8, buf);
    signer.hex2str(buf, sizeof(buf), strBuf, sizeof(strBuf));
    ASSERT_STREQ("0000000000000000000000000000000000000000000000002222222222222222", strBuf);
}

TEST_F(ContractTests, generateBytesForInt)
{
    uint8_t buf[32] = {0};
    char strBuf[65] = {0};
    // 8 bit
    int8_t value8 = -1;
    int32_t ret = Contract::generateBytesForInt(&value8, 1, buf);
    signer.hex2str(buf, sizeof(buf), strBuf, sizeof(strBuf));
    ASSERT_STREQ("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff", strBuf);
    value8 = -50;
    ret = Contract::generateBytesForInt(&value8, 1, buf);
    signer.hex2str(buf, sizeof(buf), strBuf, sizeof(strBuf));
    ASSERT_STREQ("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffce", strBuf);
    // 16 bit
    int16_t value16 = -5000;
    ret = Contract::generateBytesForInt((int8_t *)&value16, 2, buf);
    signer.hex2str(buf, sizeof(buf), strBuf, sizeof(strBuf));
    ASSERT_STREQ("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffec78", strBuf);
    value16 = 5000;
    ret = Contract::generateBytesForInt((int8_t *)&value16, 2, buf);
    signer.hex2str(buf, sizeof(buf), strBuf, sizeof(strBuf));
    ASSERT_STREQ("0000000000000000000000000000000000000000000000000000000000001388", strBuf);
    // 32 bit
    int32_t value32 = -572662306;
    ret = Contract::generateBytesForInt((int8_t *)&value32, 4, buf);
    signer.hex2str(buf, sizeof(buf), strBuf, sizeof(strBuf));
    ASSERT_STREQ("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffddddddde", strBuf);
    value32 = 572662306;
    ret = Contract::generateBytesForInt((int8_t *)&value32, 4, buf);
    signer.hex2str(buf, sizeof(buf), strBuf, sizeof(strBuf));
    ASSERT_STREQ("0000000000000000000000000000000000000000000000000000000022222222", strBuf);
    // 64 bit
    int64_t value64 = 2459565876494606882;
    ret = Contract::generateBytesForInt((int8_t *)&value64, 8, buf);
    signer.hex2str(buf, sizeof(buf), strBuf, sizeof(strBuf));
    ASSERT_STREQ("0000000000000000000000000000000000000000000000002222222222222222", strBuf);
    value64 = -2459565876494606882;
    ret = Contract::generateBytesForInt((int8_t *)&value64, 8, buf);
    signer.hex2str(buf, sizeof(buf), strBuf, sizeof(strBuf));
    ASSERT_STREQ("ffffffffffffffffffffffffffffffffffffffffffffffffddddddddddddddde", strBuf);
}

TEST_F(ContractTests, generateBytesForAddress)
{
    char addrStr[] = "89205a3a3b2a69de6dbf7f01ed13b2108b2c43e7";
    std::string expected = "000000000000000000000000" + std::string(addrStr);
    uint8_t in[20];
    signer.str2hex(addrStr, in, 20);
    uint8_t buf[32] = {0};
    char strBuf[65] = {0};
    int32_t ret = Contract::generateBytesForAddress(in, buf);
    signer.hex2str(buf, sizeof(buf), strBuf, sizeof(strBuf));
    ASSERT_STREQ(expected.c_str(), strBuf);
}

TEST_F(ContractTests, generateBytesForBool)
{
    // False
    uint8_t buf[32] = {0};
    char strBuf[65] = {0};
    Contract::generateBytesForBool(false, buf);
    signer.hex2str(buf, sizeof(buf), strBuf, sizeof(strBuf));
    ASSERT_STREQ("0000000000000000000000000000000000000000000000000000000000000000", strBuf);
    // False
    Contract::generateBytesForBool(true, buf);
    signer.hex2str(buf, sizeof(buf), strBuf, sizeof(strBuf));
    ASSERT_STREQ("0000000000000000000000000000000000000000000000000000000000000001", strBuf);
}

TEST_F(ContractTests, generateBytesForStaticBytes)
{
    uint8_t val[3] = {1, 2, 3};
    uint8_t buf[32] = {0xFF};
    char strBuf[65] = {0};
    Contract::generateBytesForStaticBytes(val, 3, buf);
    signer.hex2str(buf, sizeof(buf), strBuf, sizeof(strBuf));
    ASSERT_STREQ("0102030000000000000000000000000000000000000000000000000000000000", strBuf);
}

TEST_F(ContractTests, generateBytesForBytes)
{
    uint8_t val[3] = {1, 2, 3};
    uint8_t buf[128] = {0xFF};
    char strBuf[sizeof(buf) * 2 + 1] = {0};
    size_t encoded = Contract::generateBytesForBytes(val, 3, buf);
    signer.hex2str(buf, encoded, strBuf, sizeof(strBuf));
    ASSERT_EQ(64, encoded);
    ASSERT_STREQ("00000000000000000000000000000000000000000000000000000000000000030102030000000000000000000000000000000000000000000000000000000000", strBuf);

    uint8_t val2[] = {0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x2c, 0x20, 0x77, 0x6f, 0x72, 0x6c, 0x64, 0x21};
    encoded = Contract::generateBytesForBytes(val2, sizeof(val2), buf);
    signer.hex2str(buf, encoded, strBuf, sizeof(strBuf));
    ASSERT_EQ(64, encoded);
    ASSERT_STREQ("000000000000000000000000000000000000000000000000000000000000000d48656c6c6f2c20776f726c642100000000000000000000000000000000000000", strBuf);

    uint8_t val3[40] = {0};
    memset(val3, 0xff, sizeof(val3));
    encoded = Contract::generateBytesForBytes(val3, sizeof(val3), buf);
    signer.hex2str(buf, encoded, strBuf, sizeof(strBuf));
    ASSERT_EQ(96, encoded);
    ASSERT_STREQ("0000000000000000000000000000000000000000000000000000000000000028ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff000000000000000000000000000000000000000000000000", strBuf);
}

TEST_F(ContractTests, generateBytesForString)
{
    char val[] = "Hello, world!";
    uint8_t buf[64] = {0xFF};
    size_t encoded = Contract::generateBytesForString(val, strlen(val), buf);
    char strBuf[sizeof(buf) * 2 + 1] = {0};
    signer.hex2str(buf, sizeof(buf), strBuf, sizeof(strBuf));
    ASSERT_EQ(64, encoded);
    ASSERT_STREQ("000000000000000000000000000000000000000000000000000000000000000d48656c6c6f2c20776f726c642100000000000000000000000000000000000000", strBuf);
}

TEST_F(ContractTests, generateBytesForLongString)
{
    char val[] = "Hello, world!00000000000000000000000000000000";
    uint8_t buf[96] = {0xFF};
    size_t encoded = Contract::generateBytesForString(val, strlen(val), buf);
    char strBuf[sizeof(buf) * 2 + 1] = {0};
    signer.hex2str(buf, sizeof(buf), strBuf, sizeof(strBuf));
    ASSERT_EQ(96, encoded);
    ASSERT_STREQ("000000000000000000000000000000000000000000000000000000000000002d48656c6c6f2c20776f726c6421303030303030303030303030303030303030303030303030303030303030303000000000000000000000000000000000000000", strBuf);
}

TEST_F(ContractTests, generateBytesForStaticArrayOfStaticElements_Uint8_3)
{
    size_t elementsCount = 3;
    ParameterValue paramElements[elementsCount];
    for (int i = 0; i < elementsCount; i++)
    {
        paramElements[i].size = 1;
        paramElements[i].type = EthereumTypeName::UINT;
    }
    paramElements[0].value.uint8 = 0x01;
    paramElements[1].value.uint8 = 0x02;
    paramElements[2].value.uint8 = 0x03;

    ParameterValue paramContainer;
    paramContainer.size = elementsCount;
    paramContainer.type = EthereumTypeName::ARRAY_STATIC;
    paramContainer.value.elements = paramElements;

    std::vector<uint8_t> out;
    std::string outStr;
    size_t encoded = Contract::generateBytesForStaticArrayOfStaticElements(paramContainer, out);
    IotexHelpers.vectorToHexString(out, outStr);

    ASSERT_EQ(elementsCount * 32, encoded);
    ASSERT_STREQ("000000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000000000000020000000000000000000000000000000000000000000000000000000000000003", outStr.c_str());
}

TEST_F(ContractTests, generateBytesForStaticArrayOfStaticElements_Bytes3_2)
{
    size_t elementsCount = 2;
    ParameterValue paramElements[elementsCount];
    for (int i = 0; i < elementsCount; i++)
    {
        paramElements[i].size = 3;
        paramElements[i].type = EthereumTypeName::BYTES_STATIC;
    }
    uint8_t element0Data[] = {'a', 'b', 'c'};
    uint8_t element1Data[] = {'d', 'e', 'f'};
    paramElements[0].value.bytes = element0Data;
    paramElements[1].value.bytes = element1Data;

    ParameterValue paramContainer;
    paramContainer.size = elementsCount;
    paramContainer.type = EthereumTypeName::ARRAY_STATIC;
    paramContainer.value.elements = paramElements;

    std::vector<uint8_t> out;
    std::string outStr;
    size_t encoded = Contract::generateBytesForStaticArrayOfStaticElements(paramContainer, out);
    IotexHelpers.vectorToHexString(out, outStr);

    ASSERT_EQ(elementsCount * 32, encoded);
    ASSERT_STREQ("61626300000000000000000000000000000000000000000000000000000000006465660000000000000000000000000000000000000000000000000000000000", outStr.c_str());
}

TEST_F(ContractTests, generateBytesForStaticArrayOfDynamicElements_String3)
{
    size_t elementsCount = 3;
    ParameterValue paramElements[elementsCount];
    for (int i = 0; i < elementsCount; i++)
    {
        paramElements[i].size = 3;
        paramElements[i].type = EthereumTypeName::STRING;
    }
    std::string element0Data = "one";
    std::string element1Data = "two";
    std::string element2Data = "three";
    paramElements[0].value.string = &element0Data;
    paramElements[1].value.string = &element1Data;
    paramElements[2].value.string = &element2Data;

    ParameterValue paramContainer;
    paramContainer.size = elementsCount;
    paramContainer.type = EthereumTypeName::ARRAY_STATIC;
    paramContainer.value.elements = paramElements;

    std::vector<uint8_t> out;
    std::string outStr;
    size_t encoded = Contract::generateBytesForStaticArrayOfDynamicElements(paramContainer, out);
    IotexHelpers.vectorToHexString(out, outStr);

    char expected[] = "000000000000000000000000000000000000000000000000000000000000006000000000000000000000000000000000000000000000000000000000000000a000000000000000000000000000000000000000000000000000000000000000e000000000000000000000000000000000000000000000000000000000000000036f6e650000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000374776f000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000057468726565000000000000000000000000000000000000000000000000000000";
    ASSERT_EQ(strlen(expected) / 2, encoded);
    ASSERT_STREQ(expected, outStr.c_str());
}

TEST_F(ContractTests, generateBytesForDynamicArrayOfStaticElementsElements_Uint32_2)
{
    size_t elementsCount = 2;
    ParameterValue paramElements[elementsCount];
    for (int i = 0; i < elementsCount; i++)
    {
        paramElements[i].size = 4;
        paramElements[i].type = EthereumTypeName::UINT;
    }
    paramElements[0].value.uint32 = 1;
    paramElements[1].value.uint32 = 2;

    ParameterValue paramContainer;
    paramContainer.size = elementsCount;
    paramContainer.type = EthereumTypeName::ARRAY_DYNAMIC;
    paramContainer.value.elements = paramElements;

    std::vector<uint8_t> out;
    std::string outStr;
    size_t encoded = Contract::generateBytesForDynamicArrayOfStaticElements(paramContainer, out);
    IotexHelpers.vectorToHexString(out, outStr);

    char expected[] = "000000000000000000000000000000000000000000000000000000000000000200000000000000000000000000000000000000000000000000000000000000010000000000000000000000000000000000000000000000000000000000000002";
    // ASSERT_EQ(strlen(expected)/2, encoded);
    ASSERT_STREQ(expected, outStr.c_str());
}





// --------------------- generateFunctionSelector ------------------------------

TEST_F(ContractTests, generateFunctionSelector_Uint32AndBool)
{
    FunctionAbi function;
    function.name = "baz";
    InputOutputAbi input;
    input.name = "inputName";
    input.sizeBytes = 4;
    input.type = EthereumTypeName::UINT;
    function.inputs.push_back(input);
    InputOutputAbi input2;
    input2.name = "inputName";
    input2.type = EthereumTypeName::BOOL;
    function.inputs.push_back(input2);

    std::vector<uint8_t> out;
    Contract::generateFunctionSelector(function, out);

    char strBuf[out.size() * 2 + 1];
    memset(strBuf, 0, sizeof(strBuf));
    signer.hex2str(&out[0], out.size(), strBuf, sizeof(strBuf));

    ASSERT_STREQ("cdcd77c0", strBuf);
}

TEST_F(ContractTests, generateFunctionSelector_Uint8AndStringArray)
{
    FunctionAbi function;
    function.name = "testArray";
    InputOutputAbi input;
    input.name = "inputName";
    input.sizeBytes = 1;
    input.type = EthereumTypeName::UINT;
    function.inputs.push_back(input);
    InputOutputAbi input2;
    input2.name = "inputName";
    input2.type = EthereumTypeName::ARRAY_DYNAMIC;
    input2.arrayType = EthereumTypeName::STRING;
    function.inputs.push_back(input2);

    std::vector<uint8_t> out;
    Contract::generateFunctionSelector(function, out);

    char strBuf[out.size() * 2 + 1];
    memset(strBuf, 0, sizeof(strBuf));
    signer.hex2str(&out[0], out.size(), strBuf, sizeof(strBuf));

    ASSERT_STREQ("1b38b109", strBuf);
}

TEST_F(ContractTests, generateFunctionSelector_StaticArray_Uint8)
{
    FunctionAbi function;
    function.name = "functionName";
    InputOutputAbi input;
    input.name = "input1";
    input.type = EthereumTypeName::ARRAY_STATIC;
    input.arraySize = 2;
    input.arrayType = EthereumTypeName::UINT;
    input.arrayTypeSizeBytes = 1;
    function.inputs.push_back(input);

    std::vector<uint8_t> out;
    std::string outStr;
    Contract::generateFunctionSelector(function, out);

    IotexHelpers.vectorToHexString(out, outStr);
    ASSERT_STREQ("eacdca49", outStr.c_str());
}

TEST_F(ContractTests, generateFunctionSelector_DynamicArray_Uint8)
{
    FunctionAbi function;
    function.name = "functionName";
    InputOutputAbi input;
    input.name = "input1";
    input.type = EthereumTypeName::ARRAY_DYNAMIC;
    input.arraySize = 2;
    input.arrayType = EthereumTypeName::UINT;
    input.arrayTypeSizeBytes = 1;
    function.inputs.push_back(input);

    std::vector<uint8_t> out;
    std::string outStr;
    Contract::generateFunctionSelector(function, out);

    IotexHelpers.vectorToHexString(out, outStr);
    ASSERT_STREQ("1119273c", outStr.c_str());
}



// --------------------- generateCallData ------------------------------

TEST_F(ContractTests, generateCallData_Uint32AndBool)
{
    FunctionAbi function;
    function.name = "baz";
    InputOutputAbi input;
    input.name = "input1";
    input.sizeBytes = 4;
    input.type = EthereumTypeName::UINT;
    function.inputs.push_back(input);
    InputOutputAbi input2;
    input2.name = "input2";
    input2.type = EthereumTypeName::BOOL;
    function.inputs.push_back(input2);

    std::vector<iotex::abi::FunctionAbi> functions;
    functions.push_back(function);

    Contract contract(functions);
    ParameterValue param1;
    // param1.size = 32;
    param1.value.uint32 = 69;
    param1.size = 4;
    ParameterValue param2;
    param2.value.boolean = true;

    ParameterValuesDictionary params;
    params.AddParameter(std::string("input1"), (const ParameterValue)param1);
    params.AddParameter(std::string("input2"), (const ParameterValue)param2);
    std::string out;
    contract.generateCallData(function.name, params, out);

    std::string expected = "";
    expected += "cdcd77c0";                                                         // Function selector
    expected += "0000000000000000000000000000000000000000000000000000000000000045"; // Param 1
    expected += "0000000000000000000000000000000000000000000000000000000000000001"; // Param 2
    ASSERT_STREQ(expected.c_str(), out.c_str());
}

TEST_F(ContractTests, generateCallData_Int32)
{
    FunctionAbi function;
    function.name = "functionName";
    InputOutputAbi input;
    input.name = "input1";
    input.sizeBytes = 4;
    input.type = EthereumTypeName::INT;
    function.inputs.push_back(input);

    std::vector<iotex::abi::FunctionAbi> functions;
    functions.push_back(function);
    Contract contract(functions);

    ParameterValue param1;
    param1.value.int32 = -77;
    param1.size = 4;

    ParameterValuesDictionary params;
    params.AddParameter(std::string("input1"), (const ParameterValue)param1);

    std::string out;
    contract.generateCallData(function.name, params, out);

    char expected[] = "42b9220affffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffb3";
    ASSERT_STREQ(expected, out.c_str());
}

TEST_F(ContractTests, generateCallData_Bool)
{
    FunctionAbi function;
    function.name = "functionName";
    InputOutputAbi input;
    input.name = "input1";
    input.type = EthereumTypeName::BOOL;
    function.inputs.push_back(input);

    std::vector<iotex::abi::FunctionAbi> functions;
    functions.push_back(function);
    Contract contract(functions);

    ParameterValue param1;
    param1.value.boolean = true;

    ParameterValuesDictionary params;
    params.AddParameter(std::string("input1"), (const ParameterValue)param1);

    std::string out;
    contract.generateCallData(function.name, params, out);

    char expected[] = "551a8c310000000000000000000000000000000000000000000000000000000000000001";
    ASSERT_STREQ(expected, out.c_str());
}

TEST_F(ContractTests, generateCallData_StaticArray_Uint8)
{
    FunctionAbi function;
    function.name = "functionName";
    InputOutputAbi input;
    input.name = "input1";
    input.type = EthereumTypeName::ARRAY_STATIC;
    input.arraySize = 2;
    input.arrayType = EthereumTypeName::UINT;
    input.arrayTypeSizeBytes = 1;
    function.inputs.push_back(input);

    std::vector<iotex::abi::FunctionAbi> functions;
    functions.push_back(function);
    Contract contract(functions);

    ParameterValue paramElements[2];
    for (int i = 0; i < 2; i++)
    {
        paramElements[i].size = 1;
        paramElements[i].type = EthereumTypeName::UINT;
    }
    paramElements[0].value.uint32 = 1;
    paramElements[1].value.uint32 = 2;

    ParameterValue paramContainer;
    paramContainer.size = 2;
    paramContainer.type = EthereumTypeName::ARRAY_STATIC;
    paramContainer.value.elements = paramElements;

    ParameterValuesDictionary params;
    params.AddParameter(std::string("input1"), (const ParameterValue)paramContainer);

    std::string out;
    contract.generateCallData(function.name, params, out);

    char expected[] = "eacdca4900000000000000000000000000000000000000000000000000000000000000010000000000000000000000000000000000000000000000000000000000000002";
    ASSERT_STREQ(expected, out.c_str());
}

TEST_F(ContractTests, generateCallData_StaticArray_String)
{
    FunctionAbi function;
    function.name = "functionName";
    InputOutputAbi input;
    input.name = "input1";
    input.type = EthereumTypeName::ARRAY_STATIC;
    input.arraySize = 2;
    input.arrayType = EthereumTypeName::STRING;
    function.inputs.push_back(input);

    std::vector<iotex::abi::FunctionAbi> functions;
    functions.push_back(function);
    Contract contract(functions);

    ParameterValue paramElements[2];
    for (int i = 0; i < 2; i++)
    {
        paramElements[i].type = EthereumTypeName::STRING;
    }
    std::string value1 = "a";
    std::string value2 = "b";
    paramElements[0].value.string = &value1;
    paramElements[1].value.string = &value2;

    ParameterValue paramContainer;
    paramContainer.size = 2;
    paramContainer.type = EthereumTypeName::ARRAY_STATIC;
    paramContainer.value.elements = paramElements;

    ParameterValuesDictionary params;
    params.AddParameter(std::string("input1"), (const ParameterValue)paramContainer);

    std::string out;
    contract.generateCallData(function.name, params, out);

    char expected[] = "d4e955920000000000000000000000000000000000000000000000000000000000000020000000000000000000000000000000000000000000000000000000000000004000000000000000000000000000000000000000000000000000000000000000800000000000000000000000000000000000000000000000000000000000000001610000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000016200000000000000000000000000000000000000000000000000000000000000";
    ASSERT_STREQ(expected, out.c_str());
}

TEST_F(ContractTests, generateCallData_DynamicArray_Uint8)
{
    FunctionAbi function;
    function.name = "functionName";
    InputOutputAbi input;
    input.name = "input1";
    input.type = EthereumTypeName::ARRAY_DYNAMIC;
    input.arraySize = 2;
    input.arrayTypeSizeBytes = 1;
    input.arrayType = EthereumTypeName::UINT;
    function.inputs.push_back(input);

    std::vector<iotex::abi::FunctionAbi> functions;
    functions.push_back(function);
    Contract contract(functions);

    ParameterValue paramElements[2];
    for (int i = 0; i < 2; i++)
    {
        paramElements[i].type = EthereumTypeName::UINT;
    }
    paramElements[0].value.uint8 = 1;
    paramElements[0].size = 1;
    paramElements[1].value.uint8 = 2;
    paramElements[1].size = 1;

    ParameterValue paramContainer;
    paramContainer.size = 2;
    paramContainer.type = EthereumTypeName::ARRAY_DYNAMIC;
    paramContainer.value.elements = paramElements;

    ParameterValuesDictionary params;
    params.AddParameter(std::string("input1"), (const ParameterValue)paramContainer);

    std::string out;
    contract.generateCallData(function.name, params, out);

    char expected[] = "1119273c0000000000000000000000000000000000000000000000000000000000000020000000000000000000000000000000000000000000000000000000000000000200000000000000000000000000000000000000000000000000000000000000010000000000000000000000000000000000000000000000000000000000000002";
    ASSERT_STREQ(expected, out.c_str());
}
