#include "helpers/json_helper.h"

using namespace iotex;
using namespace iotex::json;

ResultCode iotex::json::SetValueFromJsonObject(const cJSON* json, reflection::CppType type,
											   void* pData, size_t max_size)
{
	// Validate type and size
	switch(type)
	{
		case reflection::CppType::OBJECT:
			if(!cJSON_IsObject(json))
			{
				return iotex::ResultCode::ERROR_JSON_PARSE;
			}
			break;

		case reflection::CppType::BOOLEAN:
			if(!cJSON_IsBool(json))
			{
				return iotex::ResultCode::ERROR_JSON_PARSE;
			}
			break;

		case reflection::CppType::C_STRING:
		case reflection::CppType::STRING:
			if(!cJSON_IsString(json) || (max_size > 0 && strlen(json->valuestring) > max_size))
			{
				return ResultCode::ERROR_JSON_PARSE;
			}
			break;

		case reflection::CppType::UINT8:
		case reflection::CppType::UINT16:
		case reflection::CppType::UINT32:
		case reflection::CppType::UINT64:
			if(!cJSON_IsNumber(json))
			{
				return iotex::ResultCode::ERROR_JSON_PARSE;
			}
			break;

		default:
			return iotex::ResultCode::ERROR_JSON_PARSE;
	}

	// Set the value
	switch(type)
	{
		case reflection::CppType::C_STRING:
			memcpy(pData, json->valuestring, strlen(json->valuestring));
			break;

		case reflection::CppType::STRING:
			*(IotexString*)pData = json->valuestring;
			break;

		case reflection::CppType::BOOLEAN:
			*(bool*)pData = json->valueint;
			break;

		case reflection::CppType::UINT8:
			*(uint8_t*)pData = json->valueint;
			break;

		case reflection::CppType::UINT16:
			*(uint16_t*)pData = json->valueint;
			break;

		case reflection::CppType::UINT32:
			*(uint32_t*)pData = json->valueint;
			break;

		case reflection::CppType::UINT64:
			*(uint64_t*)pData = json->valueint;
			break;

		case reflection::CppType::BIGINT:
		case reflection::CppType::OBJECT:
		case reflection::CppType::BYTES:
		case reflection::CppType::ARRAY:
			break;
	}

	return iotex::ResultCode::SUCCESS;
}