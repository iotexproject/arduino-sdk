#ifndef IOTEX_CONTRACT_H
#define IOTEX_CONTRACT_H

#include "abi/abi.h"
#include <map>
#include <stddef.h>
#include <stdint.h>
#include <vector>

namespace iotex
{

class ParameterValuesDictionary
{
  public:
	void AddParameter(const IotexString& name, iotex::abi::ParameterValue value);
	bool ContainsParameter(const IotexString& name);
	iotex::abi::ParameterValue GetParameter(const IotexString& name);

  private:
	std::map<const IotexString, iotex::abi::ParameterValue> _dictionary;
};

class Contract
{
  public:
	/**
	 * @brief Construct a new contract object passing the ABI JSON string
	 *
	 * @param abiJson The ABI as a serialized JSON string
	 */
	Contract(IotexString& abiJson);

	/**
	 * @brief Construct a new Contract object from a vector of FunctionAbi objects
	 *
	 * @param functions The contract functions
	 */
	Contract(std::vector<iotex::abi::FunctionAbi>& functions);

	/**
	 * @brief Generates the Ethereum encoded data for the contract call
	 *
	 * @param functionName The function name
	 * @param params The paremeters values
	 * @param out[out] The encoded data as a hex string if successful
	 * @return ResultCode Success or error code
	 */
	ResultCode generateCallData(const IotexString& functionName, ParameterValuesDictionary& params,
								IotexString& out);

	static int32_t generateBytesForUint(const uint8_t* pVal, size_t size, uint8_t* out);
	static int32_t generateBytesForInt(const int8_t* pVal, size_t size, uint8_t* out);
	static int32_t generateBytesForAddress(const uint8_t* pVal, uint8_t* out);
	static int32_t generateBytesForBool(bool val, uint8_t* out);
	static int32_t generateBytesForStaticBytes(uint8_t* pVal, size_t size, uint8_t* out);
	static int32_t generateBytesForBytes(uint8_t* pVal, size_t size, uint8_t* out);
	static int32_t generateBytesForString(char* pVal, size_t size, uint8_t* out);
	static int32_t generateBytesForStaticArrayOfStaticElements(iotex::abi::ParameterValue param,
															   std::vector<uint8_t>& data);
	static int32_t generateBytesForDynamicArrayOfStaticElements(iotex::abi::ParameterValue param,
																std::vector<uint8_t>& data);
	static int32_t generateBytesForStaticArrayOfDynamicElements(iotex::abi::ParameterValue param,
																std::vector<uint8_t>& data);
	static void generateFunctionSelector(iotex::abi::FunctionAbi& function,
										 std::vector<uint8_t>& out);

	// Not supported:
	// Nested arrays
	// Tuple
	// Fixed

  private:
	std::vector<iotex::abi::FunctionAbi> _functions;

	static int32_t generateBytesForParameter(iotex::abi::ParameterValue param,
											 iotex::abi::EthereumTypeName type,
											 std::vector<uint8_t>& data);
	static int32_t generateBytesForSimpleType(iotex::abi::ParameterValue param,
											  iotex::abi::EthereumTypeName type, uint8_t buf[32]);
};
} // namespace iotex

#endif
