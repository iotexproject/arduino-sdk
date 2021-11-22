#ifndef IOTEX_ENCODER_H
#define IOTEX_ENCODER_H

#include "IoTeXConstants.h"
#include "IoTeXResultCodes.h"
#include "protobuf/pb_api.h"
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>

namespace iotex
{
class Encoder
{
  public:
	// Bech32
	ResultCode bech32_encodeHexString(const IotexString& data, IotexString& encodedData);
	ResultCode bech32_encode(const std::vector<uint8_t> data, IotexString& encodedData);
	ResultCode bech32_convertBytesTo5Bit(const std::vector<uint8_t>& in, std::vector<uint8_t>& out);

	// Base64
	ResultCode base64_encode(const uint8_t* data, size_t size, char* out);

	// Protobuf
	int32_t protobuf_encodeTransfer(responsetypes::ActionCore_Transfer& transfer, uint8_t* out,
									size_t maxOutSize);
	int32_t protobuf_encodeExecution(responsetypes::ActionCore_Execution& execution, uint8_t* out,
									 size_t maxOutSize);

  private:
};
} // namespace iotex

// Declare global encoder
extern iotex::Encoder encoder;

#endif