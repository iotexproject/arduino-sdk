#ifndef HTTP_H
#define HTTP_H

#include "helpers/client_helper.h"
#include "IoTeXResultCodes.h"
#include <memory>
#include <string>

namespace iotex
{
enum GrpcStatusCode
{
	OK = 0,
	CANCELLED,
	UNKNOWN,
	INVALIUD_ARGUMENT,
	DEADLINE_EXCEEDED,
	NOT_FOUND,
	ALREADY_EXISTS,
	PERMISSION_DENIED,
	RESOURCE_EXHAUSTED,
	FAILED_PRECONDITION,
	ABORTED,
	OUT_OF_RANGE,
	UNIMPLEMENTED,
	INTERNAL,
	UNAVAILABLE,
	DATA_LOSS,
	UNAUTHENTICATED,
	END
};

struct GrpcStatus
{
	GrpcStatusCode code;
	IotexString message;
};

class IHTTP
{
  protected:
	IHTTP() = default;

  public:
	virtual ~IHTTP()
	{
	}

	virtual IotexString get(const char* request) = 0;
	virtual iotex::ResultCode post(const char* request, const char* body,
								   IotexString& response) = 0;

	virtual int get(const char* request, char* rspBuf, size_t size)
	{
		return -1;
	};
	virtual int post(const char* request, const char* body, char* rspBuf, size_t size)
	{
		return -1;
	};
};

class AbstractHTTP : public IHTTP
{
  protected:
	AbstractHTTP() = default;
	AbstractHTTP(AbstractHTTP&&) = delete;
	AbstractHTTP& operator=(AbstractHTTP&&) = delete;
	AbstractHTTP& operator=(const AbstractHTTP& other) = default;

  public:
	virtual ~AbstractHTTP(){};
};

std::unique_ptr<IHTTP> makeHTTP();

} // namespace iotex

#endif
