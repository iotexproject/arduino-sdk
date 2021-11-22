#ifndef JSON_HELPER_H
#define JSON_HELPER_H

#include "extern/cjson/cJSON.h"
#include "helpers/client_helper.h"
#include "helpers/reflection_helper.h"
#include "IoTeXResultCodes.h"
#include <string.h>
#include <string>

namespace iotex
{
namespace json
{
struct JsonProp
{
	const reflection::CppType type;
	const IotexString name;

	JsonProp(reflection::CppType type, IotexString name) :
		type(type), name(name), _initialized(false)
	{
	}

	// Move constructor
	JsonProp(JsonProp&& orig) noexcept : type(orig.type), name(orig.name)
	{
		_initialized = orig._initialized;
		_bytesCount = orig._bytesCount;
		if(_initialized)
		{
			_value.object = orig._value.object;
			orig._value.object = nullptr;
		}
	}

	/* Setters */
	void setValue(const char* value)
	{
		if(_initialized && type == reflection::CppType::C_STRING)
		{
			free(_value.c_string);
		}
		_value.c_string = (char*)malloc(strlen(value));
		memcpy(_value.c_string, value, strlen(value));
		_initialized = true;
	}

	void setValue(const IotexString& value)
	{
		if(_initialized && type == reflection::CppType::STRING)
		{
			free(_value.string);
		}
		_value.string = new IotexString(value);
		_initialized = true;
	}

	void setValue(uint64_t value)
	{
		_initialized = true;
		_value.uint64 = value;
	}

	void setValue(bool value)
	{
		_initialized = true;
		_value.boolean = value;
	}

	/* Getters */
	const char* getValueCString()
	{
		return _initialized ? _value.c_string : nullptr;
	}

	const IotexString* getValueString()
	{
		return _initialized ? _value.string : nullptr;
	}

	uint64_t getValueUint64()
	{
		return _value.uint64;
	}

	bool getValueBool()
	{
		return _value.boolean;
	}

	const uint8_t* getValueBytes()
	{
		return _value.bytes;
	}

	~JsonProp()
	{
		if(_initialized && isHeapAllocated())
		{
			free(_value.object);
		}
	}

	uint32_t getBytesCount()
	{
		return _bytesCount;
	}

	void ToString(IotexString& out, bool enclose = false)
	{
		out = "";
		if(enclose)
			out += "{";
		// Name
		out += "\"" + name + "\":";
		// Value
		appendValueToString(out);
		if(enclose)
			out += "}";
	}

  protected:
	bool _initialized;
	union
	{
		uint8_t uint8;
		uint16_t uint16;
		uint32_t uint32;
		uint64_t uint64;
		bool boolean;
		char* c_string;
		IotexString* string;
		void* object;
		uint8_t* bytes;
	} _value;

	uint32_t _bytesCount; // bytes count, only used for bytes type

	bool isHeapAllocated()
	{
		if(type == reflection::CppType::C_STRING || type == reflection::CppType::STRING ||
		   type == reflection::CppType::OBJECT || type == reflection::CppType::BYTES)
		{
			return true;
		}
		else
			return false;
	}

	virtual void appendValueToString(IotexString& out)
	{
		// TODO Make pure abstract
	}
};

/**
 * @struct  StringJsonProp  Struct that represents a string json property using
 * IotexString
 */
struct StringJsonProp : JsonProp
{
	StringJsonProp(IotexString name) : JsonProp(reflection::CppType::STRING, name)
	{
	}

	StringJsonProp(StringJsonProp&& orig) noexcept : JsonProp(std::move(orig))
	{
	}

	const IotexString* getValue()
	{
		return _value.string;
	}

  private:
	void appendValueToString(IotexString& out) override
	{
		out += "\"";
		out += _value.string->c_str();
		out += "\"";
	}
};

struct Uint8JsonProp : JsonProp
{
	Uint8JsonProp(IotexString name) : JsonProp(reflection::CppType::UINT8, name)
	{
	}

	Uint8JsonProp(Uint8JsonProp&& orig) noexcept : JsonProp(std::move(orig))
	{
	}

	uint8_t getValue()
	{
		return _value.uint8;
	}

  private:
	void appendValueToString(IotexString& out) override
	{
		char buf[4] = {0};
		sprintf(buf, "%u", _value.uint8);
		out += buf;
	}
};

struct Uint64JsonProp : JsonProp
{
	Uint64JsonProp(IotexString name) : JsonProp(reflection::CppType::UINT64, name)
	{
	}

	Uint64JsonProp(Uint64JsonProp&& orig) noexcept : JsonProp(std::move(orig))
	{
	}

	uint64_t getValue()
	{
		return _value.uint64;
	}

  private:
	void appendValueToString(IotexString& out) override
	{
		char buf[21] = {0};
		sprintf(buf, "%lu", _value.uint64);
		out += buf;
	}
};

struct BytesJsonProp : JsonProp
{
	BytesJsonProp(IotexString name) : JsonProp(reflection::CppType::BYTES, name)
	{
	}

	const uint8_t* getValue()
	{
		return _value.bytes;
	}
};

struct BoolJsonProp : JsonProp
{
	BoolJsonProp(IotexString name) : JsonProp(reflection::CppType::BOOLEAN, name)
	{
	}

	BoolJsonProp(BoolJsonProp&& orig) noexcept : JsonProp(std::move(orig))
	{
	}

	bool getValue()
	{
		return _value.boolean;
	}

  private:
	void appendValueToString(IotexString& out)
	{
		out += _value.boolean ? "true" : "false";
	}
};

struct ArrayJsonProp : JsonProp
{
	ArrayJsonProp(IotexString name) : JsonProp(reflection::CppType::ARRAY, name)
	{
	}
};

iotex::ResultCode SetValueFromJsonObject(const cJSON* json, reflection::CppType type, void* pData,
										 size_t max_size = 0);
} // namespace json
}; // namespace iotex

#endif