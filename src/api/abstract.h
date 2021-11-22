#ifndef ABSTRACT_H
#define ABSTRACT_H

#include "host/host.h"
#include "http/http.h"

namespace iotex
{
namespace api
{
class Abstract
{
  public:
	void setHost(const char* newHost, int newPort, const char* baseUrl = nullptr)
	{
		this->_host.set(newHost, newPort, baseUrl);
	};

  protected:
	Host _host;
	std::unique_ptr<IHTTP> _http;

	Abstract() : _http(makeHTTP())
	{
	}
	explicit Abstract(IHTTP* http) : _http(http)
	{
	}
}; // class Abstract

} // namespace api
} // namespace iotex

#endif
