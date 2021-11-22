#ifndef CONNECTION_H
#define CONNECTION_H

#include "api/abstract.h"
#include "api/api.h"
#include "host/host.h"
#include "http/http.h"

namespace iotex
{
template<typename TAPI>
class Connection
{
  public:
	TAPI api;
	Host host;

	Connection() = default;
	explicit Connection(const TAPI& other) : api(other)
	{
	}

	Connection(const char* newIP, int newPort, const char* baseUrl = nullptr)
	{
		this->host.set(newIP, newPort, baseUrl);
		this->api.setHost(newIP, newPort, baseUrl);
	};
};

} // namespace iotex

#endif
