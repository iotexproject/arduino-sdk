#ifndef IOTEX_ABI_H
#define IOTEX_ABI_H

#include "extern/cjson/cJSON.h"
#include "helpers/client_helper.h"
#include "helpers/json_helper.h"
#include "IoTeXConstants.h"
#include "IoTeXResultCodes.h"
#include <vector>

namespace iotex
{
namespace abi
{
/**
 * @brief Enum that contains the Ethereum types
 *
 */
enum EthereumTypeName
{
	UINT,
	INT,
	ADDRESS,
	BOOL,
	FIXED,
	UFIXED,
	BYTES_STATIC,
	STRING,
	BYTES_DYNAMIC,
	ARRAY_STATIC,
	ARRAY_DYNAMIC,
	TUPLE_STATIC,
	TUPLE_DYNAMIC,
	UNKNOWN
};

/**
 * @brief String lookup table for EthereumTypeName
 *
 */
static const char* EthereumTypeNameToStringLT[] = {"uint",	 "int",	  "address", "bool",  "fixed",
												   "ufixed", "bytes", "string",	 "bytes", "",
												   "",		 "",	  ""};

// Json objects
/**
 * @brief A class that represents the ABI for an input or output field of a
 * contract funciton
 *
 */
class InputOutputAbi
{
  public:
	IotexString name;
	EthereumTypeName type;
	uint32_t sizeBytes;
	EthereumTypeName arrayType;
	uint32_t arrayTypeSizeBytes = 0;
	uint32_t arraySize;

	/**
	 * @brief Parses the size and type from the input/output "type" property in
	 * the ABI JSON
	 *
	 * @param str
	 * @param out
	 * @return ResultCode
	 */
	static ResultCode getTypeAndSizeFromString(IotexString& str, InputOutputAbi& out);

	/**
	 * @brief Checks whether the type of this input or output is dynamic or static
	 *
	 * @return true if the type is dynamic
	 * @return false if the type is static
	 */
	bool IsDynamic()
	{
		bool isDynamic = false;
		if(type == EthereumTypeName::BYTES_DYNAMIC)
			isDynamic = true;
		else if(type == EthereumTypeName::STRING)
			isDynamic = true;
		else if(type == EthereumTypeName::ARRAY_DYNAMIC)
			isDynamic = true;
		else if(type == EthereumTypeName::TUPLE_DYNAMIC)
			isDynamic = true;
		else if(type == EthereumTypeName::ARRAY_STATIC)
		{
			if(arrayType == EthereumTypeName::BYTES_DYNAMIC ||
			   arrayType == EthereumTypeName::STRING ||
			   arrayType == EthereumTypeName::ARRAY_DYNAMIC ||
			   arrayType == EthereumTypeName::TUPLE_DYNAMIC)
				isDynamic = true;
		}
		return isDynamic;
	}

	/**
	 * @brief Checks if the type of this input or output is an integer (int or
	 * uint) type of any size
	 *
	 * @return true if the type is an integer
	 * @return false if the type is not an integer
	 */
	bool isInteger() const
	{
		return (type == EthereumTypeName::UINT || type == EthereumTypeName::INT);
	}

  private:
	static ResultCode getSizeFromStringAndCheckIfArray(IotexString& str, InputOutputAbi& out);
};

/**
 * @brief A class that represents the ABI for a contract function
 *
 */
class FunctionAbi
{
  public:
	bool constant;
	std::vector<InputOutputAbi> inputs; // Not present in fallback
	IotexString name;
	std::vector<InputOutputAbi> outputs; // Not present in constructor or fallback
	IotexString stateMutability;
	bool payable;

	/**
	 * @brief Gets the funciton signature as a string
	 *
	 * @param out
	 */
	void getSignature(IotexString& out);
};

// Parsing
/**
 * @brief Parses the serialized json ABI of a contract
 *
 * @param contractAbi The ABI as a serialized JSON object
 * @param out A container with all the parsed ABI functions
 * @return ResultCode Succes or an error code
 */
ResultCode parseContract(const IotexString& contractAbi, std::vector<FunctionAbi>& out);

/**
 * @brief Parses a json function of a contract
 *
 * @param data The funcion as a JSON object
 * @param out The parsed function
 * @return ResultCode Succes or an error code
 */
ResultCode parseFunction(const cJSON* data, FunctionAbi& out);

/**
 * @brief Parses a json input or putput of a contract function
 *
 * @param data The nput or output as a JSON object
 * @param out The parsed input or output
 * @return ResultCode  Succes or an error code
 */
ResultCode parseInputOutput(const cJSON* data, InputOutputAbi& out);

// Utils
/**
 * @brief Class that represents the value of a contract function input or output
 * parameter
 *
 */
class ParameterValue
{
  public:
	union
	{
		uint8_t uint8;
		uint16_t uint16;
		uint32_t uint32;
		uint64_t uint64;
		int8_t int8;
		int16_t int16;
		int32_t int32;
		int64_t int64;
		IotexString* uint256;
		bool boolean;
		IotexString* string;
		uint8_t* bytes;
		ParameterValue* elements; // Array or tuple elements
	} value;
	int32_t size = -1;
	bool isBigInt = false;
	EthereumTypeName type = EthereumTypeName::UNKNOWN;

	/**
	 * @brief Checks if this parameter is dynamic
	 *
	 * @return true if thhis parameter is dynamic
	 * @return false if thhis parameter is static
	 */
	bool isDynamic()
	{
		return (type == EthereumTypeName::BYTES_DYNAMIC || type == EthereumTypeName::STRING ||
				type == EthereumTypeName::ARRAY_DYNAMIC || type == EthereumTypeName::TUPLE_DYNAMIC);
	}
};

/**
 * @brief Generates a uint ParameterValue from a uint64_t value
 *
 * @param value
 * @return ParameterValue
 */
ParameterValue MakeParamUint(uint64_t value);

/**
 * @brief Generates a uint ParameterValue from a int64_t value
 *
 * @param value
 * @return ParameterValue
 */
ParameterValue MakeParamInt(int64_t value);

/**
 * @brief Generates a uint ParameterValue from a string value
 *
 * @param value
 * @return ParameterValue
 */
ParameterValue MakeParamString(IotexString& value);

/**
 * @brief Generates a uint ParameterValue from an address
 *
 * @param value
 * @return ParameterValue
 */
ParameterValue MakeParamAddress(uint8_t value[ETH_ADDRESS_SIZE]);

/**
 * @brief Generates a uint ParameterValue from a byte array
 *
 * @param value The byte array
 * @param size The size of the byte array
 * @param dynamic Whether the created Parameter is dynamic or not
 * @return ParameterValue
 */
ParameterValue MakeParamBytes(uint8_t value[], size_t size, bool dynamic);
} // namespace abi
} // namespace iotex

#endif
