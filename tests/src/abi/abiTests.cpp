#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "extern/cjson/cJSON.h"
#include "helpers/json_helper.h"
#include "abi/abi.h"

using namespace std;
using namespace testing;
using namespace iotex;
using namespace iotex::abi;

class AbiTests : public Test
{
    void SetUp() override {}
    
    void TearDown() override {}

};





// --------------------- parseInputOutput  ------------------------------

TEST_F(AbiTests, ParseInputOutput_UintX)
{
    std::vector<std::string> sizesStr;
    std::vector<uint16_t> sizes;
    for (uint16_t i = 8; i <= 256; i+=8)
    {
        sizes.push_back(i);
        std::string str = "uint" + std::to_string(i);
        sizesStr.push_back(str);
    }

    for (int i = 0; i < sizes.size(); i++)
    {
        cJSON *input = cJSON_CreateObject();
        cJSON_AddStringToObject(input, "name", "inputName");
        cJSON_AddStringToObject(input, "type", sizesStr[i].c_str());
        InputOutputAbi parsedInput;
        ResultCode result = parseInputOutput(input, parsedInput);

        ASSERT_EQ(result, ResultCode::SUCCESS);
        ASSERT_STREQ(parsedInput.name.c_str(), "inputName");
        ASSERT_EQ(EthereumTypeName::UINT, parsedInput.type);
        ASSERT_EQ(sizes[i]/8, parsedInput.sizeBytes);
        cJSON_Delete(input);
    }
}

TEST_F(AbiTests, ParseInputOutput_Uint)
{
    cJSON *input = cJSON_CreateObject();
    cJSON_AddStringToObject(input, "name", "inputName");
    cJSON_AddStringToObject(input, "type", "uint");
    InputOutputAbi parsedInput;
    ResultCode result = parseInputOutput(input, parsedInput);

    ASSERT_EQ(result, ResultCode::SUCCESS);
    ASSERT_STREQ(parsedInput.name.c_str(), "inputName");
    ASSERT_EQ(EthereumTypeName::UINT, parsedInput.type);
    ASSERT_EQ(32, parsedInput.sizeBytes);
    cJSON_Delete(input);
}

TEST_F(AbiTests, ParseInputOutput_IntX)
{
    std::vector<std::string> sizesStr;
    std::vector<uint16_t> sizes;
    for (uint16_t i = 8; i <= 256; i+=8)
    {
        sizes.push_back(i);
        std::string str = "int" + std::to_string(i);
        sizesStr.push_back(str);
    }

    for (int i = 0; i < sizes.size(); i++)
    {
        cJSON *input = cJSON_CreateObject();
        cJSON_AddStringToObject(input, "name", "inputName");
        cJSON_AddStringToObject(input, "type", sizesStr[i].c_str());
        InputOutputAbi parsedInput;
        ResultCode result = parseInputOutput(input, parsedInput);

        ASSERT_EQ(result, ResultCode::SUCCESS);
        ASSERT_STREQ(parsedInput.name.c_str(), "inputName");
        ASSERT_EQ(EthereumTypeName::INT, parsedInput.type);
        ASSERT_EQ(sizes[i]/8, parsedInput.sizeBytes);
        cJSON_Delete(input);
    }
}

TEST_F(AbiTests, ParseInputOutput_Int)
{
    cJSON *input = cJSON_CreateObject();
    cJSON_AddStringToObject(input, "name", "inputName");
    cJSON_AddStringToObject(input, "type", "int");
    InputOutputAbi parsedInput;
    ResultCode result = parseInputOutput(input, parsedInput);

    ASSERT_EQ(result, ResultCode::SUCCESS);
    ASSERT_STREQ(parsedInput.name.c_str(), "inputName");
    ASSERT_EQ(EthereumTypeName::INT, parsedInput.type);
    ASSERT_EQ(32, parsedInput.sizeBytes);
    cJSON_Delete(input);
}

TEST_F(AbiTests, ParseInputOutput_Address)
{
    cJSON *input = cJSON_CreateObject();
    cJSON_AddStringToObject(input, "name", "inputName");
    cJSON_AddStringToObject(input, "type", "address");
    InputOutputAbi parsedInput;
    ResultCode result = parseInputOutput(input, parsedInput);

    ASSERT_EQ(result, ResultCode::SUCCESS);
    ASSERT_STREQ(parsedInput.name.c_str(), "inputName");
    ASSERT_EQ(EthereumTypeName::ADDRESS, parsedInput.type);
    cJSON_Delete(input);
}

TEST_F(AbiTests, ParseInputOutput_Bool)
{
    cJSON *input = cJSON_CreateObject();
    cJSON_AddStringToObject(input, "name", "inputName");
    cJSON_AddStringToObject(input, "type", "bool");
    InputOutputAbi parsedInput;
    ResultCode result = parseInputOutput(input, parsedInput);

    ASSERT_EQ(result, ResultCode::SUCCESS);
    ASSERT_STREQ(parsedInput.name.c_str(), "inputName");
    ASSERT_EQ(EthereumTypeName::BOOL, parsedInput.type);
    cJSON_Delete(input);
}

TEST_F(AbiTests, ParseInputOutput_StaticBytes)
{
    std::vector<std::string> sizesStr;
    std::vector<uint16_t> sizes;
    for (uint16_t i = 1; i <= 32; i++)
    {
        sizes.push_back(i);
        std::string str = "bytes" + std::to_string(i);
        sizesStr.push_back(str);
    }

    for (int i = 0; i < sizes.size(); i++)
    {
        cJSON *input = cJSON_CreateObject();
        cJSON_AddStringToObject(input, "name", "inputName");
        cJSON_AddStringToObject(input, "type", sizesStr[i].c_str());
        InputOutputAbi parsedInput;
        ResultCode result = parseInputOutput(input, parsedInput);

        ASSERT_EQ(result, ResultCode::SUCCESS);
        ASSERT_STREQ(parsedInput.name.c_str(), "inputName");
        ASSERT_EQ(EthereumTypeName::BYTES_STATIC, parsedInput.type);
        ASSERT_EQ(sizes[i], parsedInput.sizeBytes);
        cJSON_Delete(input);
    }
}

TEST_F(AbiTests, ParseInputOutput_DynamicBytes)
{
    cJSON *input = cJSON_CreateObject();
    cJSON_AddStringToObject(input, "name", "inputName");
    cJSON_AddStringToObject(input, "type", "bytes");
    InputOutputAbi parsedInput;
    ResultCode result = parseInputOutput(input, parsedInput);

    ASSERT_EQ(result, ResultCode::SUCCESS);
    ASSERT_STREQ(parsedInput.name.c_str(), "inputName");
    ASSERT_EQ(EthereumTypeName::BYTES_DYNAMIC, parsedInput.type);
    cJSON_Delete(input);
}

TEST_F(AbiTests, ParseInputOutput_String)
{
    cJSON *input = cJSON_CreateObject();
    cJSON_AddStringToObject(input, "name", "inputName");
    cJSON_AddStringToObject(input, "type", "string");
    InputOutputAbi parsedInput;
    ResultCode result = parseInputOutput(input, parsedInput);

    ASSERT_EQ(result, ResultCode::SUCCESS);
    ASSERT_STREQ(parsedInput.name.c_str(), "inputName");
    ASSERT_EQ(EthereumTypeName::STRING, parsedInput.type);
    cJSON_Delete(input);
}

TEST_F(AbiTests, ParseInputOutput_DynamicArrayOfDynamicBytes)
{
    cJSON *input = cJSON_CreateObject();
    cJSON_AddStringToObject(input, "name", "inputName");
    cJSON_AddStringToObject(input, "type", "bytes[]");
    InputOutputAbi parsedInput;
    ResultCode result = parseInputOutput(input, parsedInput);

    ASSERT_EQ(result, ResultCode::SUCCESS);
    ASSERT_STREQ(parsedInput.name.c_str(), "inputName");
    ASSERT_EQ(EthereumTypeName::ARRAY_DYNAMIC, parsedInput.type);
    ASSERT_EQ(EthereumTypeName::BYTES_DYNAMIC, parsedInput.arrayType);
    cJSON_Delete(input);
}

TEST_F(AbiTests, ParseInputOutput_DynamicArrayOfStaticBytes)
{
    cJSON *input = cJSON_CreateObject();
    cJSON_AddStringToObject(input, "name", "inputName");
    cJSON_AddStringToObject(input, "type", "bytes5[]");
    InputOutputAbi parsedInput;
    ResultCode result = parseInputOutput(input, parsedInput);

    ASSERT_EQ(result, ResultCode::SUCCESS);
    ASSERT_STREQ(parsedInput.name.c_str(), "inputName");
    ASSERT_EQ(EthereumTypeName::ARRAY_DYNAMIC, parsedInput.type);
    ASSERT_EQ(EthereumTypeName::BYTES_STATIC, parsedInput.arrayType);
    ASSERT_EQ(5, parsedInput.arrayTypeSizeBytes);
    cJSON_Delete(input);
}


// TODO Arrays, tuples, fixed, ufixed



// --------------------- getSignature ------------------------------

TEST_F(AbiTests, FunctionAbi_GetSignature_Uint)
{
    FunctionAbi function;
    function.name = "functionName";
    InputOutputAbi input;
    input.name = "inputName";
    input.type = EthereumTypeName::UINT;
    input.sizeBytes = 32;
    function.inputs.push_back(input);

    std::string buf;
    std::string expected = R"(functionName(uint256))";
    function.getSignature(buf);
    ASSERT_STREQ(expected.c_str(), buf.c_str());
}

TEST_F(AbiTests, FunctionAbi_GetSignature_Int)
{
    FunctionAbi function;
    function.name = "functionName";
    InputOutputAbi input;
    input.name = "inputName";
    input.type = EthereumTypeName::INT;
    input.sizeBytes = 1;
    function.inputs.push_back(input);

    std::string buf;
    std::string expected = R"(functionName(int8))";
    function.getSignature(buf);
    ASSERT_STREQ(expected.c_str(), buf.c_str());
}

TEST_F(AbiTests, FunctionAbi_GetSignature_Bool)
{
    FunctionAbi function;
    function.name = "functionName";
    InputOutputAbi input;
    input.name = "inputName";
    input.type = EthereumTypeName::BOOL;
    function.inputs.push_back(input);

    std::string buf;
    std::string expected = R"(functionName(bool))";
    function.getSignature(buf);
    ASSERT_STREQ(expected.c_str(), buf.c_str());
}

TEST_F(AbiTests, FunctionAbi_GetSignature_Bytes32)
{
    FunctionAbi function;
    function.name = "functionName";
    InputOutputAbi input;
    input.name = "inputName";
    input.type = EthereumTypeName::BYTES_STATIC;
    input.sizeBytes = 32;
    function.inputs.push_back(input);

    std::string buf;
    std::string expected = R"(functionName(bytes32))";
    function.getSignature(buf);
    ASSERT_STREQ(expected.c_str(), buf.c_str());
}

TEST_F(AbiTests, FunctionAbi_GetSignature_Bytes)
{
    FunctionAbi function;
    function.name = "functionName";
    InputOutputAbi input;
    input.name = "inputName";
    input.type = EthereumTypeName::BYTES_DYNAMIC;
    function.inputs.push_back(input);

    std::string buf;
    std::string expected = R"(functionName(bytes))";
    function.getSignature(buf);
    ASSERT_STREQ(expected.c_str(), buf.c_str());
}

TEST_F(AbiTests, FunctionAbi_GetSignature_StaticArrayOfDynamicBytes)
{
    FunctionAbi function;
    function.name = "functionName";
    InputOutputAbi input;
    input.name = "inputName";
    input.type = EthereumTypeName::ARRAY_STATIC;
    input.arrayType = EthereumTypeName::BYTES_DYNAMIC;
    input.arraySize = 3;
    function.inputs.push_back(input);

    std::string buf;
    std::string expected = R"(functionName(bytes[3]))";
    function.getSignature(buf);
    ASSERT_STREQ(expected.c_str(), buf.c_str());
}

TEST_F(AbiTests, FunctionAbi_GetSignature_StaticArrayOfStaticBytes)
{
    FunctionAbi function;
    function.name = "functionName";
    InputOutputAbi input;
    input.name = "inputName";
    input.type = EthereumTypeName::ARRAY_STATIC;
    input.arrayType = EthereumTypeName::BYTES_DYNAMIC;
    input.arrayTypeSizeBytes = 5;
    input.arraySize = 3;
    function.inputs.push_back(input);

    std::string buf;
    std::string expected = R"(functionName(bytes5[3]))";
    function.getSignature(buf);
    ASSERT_STREQ(expected.c_str(), buf.c_str());
}

TEST_F(AbiTests, FunctionAbi_GetSignature_DynamicArrayOfStaticBytes)
{
    FunctionAbi function;
    function.name = "functionName";
    InputOutputAbi input;
    input.name = "inputName";
    input.type = EthereumTypeName::ARRAY_DYNAMIC;
    input.arrayType = EthereumTypeName::BYTES_STATIC;
    input.arrayTypeSizeBytes = 5;
    input.arraySize = 3;
    function.inputs.push_back(input);

    std::string buf;
    std::string expected = R"(functionName(bytes5[]))";
    function.getSignature(buf);
    ASSERT_STREQ(expected.c_str(), buf.c_str());
}

TEST_F(AbiTests, FunctionAbi_GetSignature_IntAndUintAndBytesAndBytes10AndBool)
{
    FunctionAbi function;
    function.name = "functionName";
    InputOutputAbi input;
    input.name = "inputName";
    input.sizeBytes = 32;
    input.type = EthereumTypeName::INT;
    function.inputs.push_back(input);
    InputOutputAbi input2;
    input2.sizeBytes = 32;
    input2.name = "inputName";
    input2.type = EthereumTypeName::UINT;
    function.inputs.push_back(input2);
    InputOutputAbi input3;
    input3.name = "inputName";
    input3.type = EthereumTypeName::BYTES_DYNAMIC;
    function.inputs.push_back(input3);
    InputOutputAbi input4;
    input4.name = "inputName";
    input4.type = EthereumTypeName::BYTES_STATIC;
    input4.sizeBytes = 10;
    function.inputs.push_back(input4);
    InputOutputAbi input5;
    input5.name = "inputName";
    input5.type = EthereumTypeName::BOOL;
    function.inputs.push_back(input5);

    std::string buf;
    std::string expected = R"(functionName(int256,uint256,bytes,bytes10,bool))";
    function.getSignature(buf);
    ASSERT_STREQ(expected.c_str(), buf.c_str());
}




// --------------------- getSizeFromStringAndCheckIfArray ------------------------------

TEST_F(AbiTests, FunctionAbi_getSizeFromStringAndCheckIfArray_Uint8)
{

}