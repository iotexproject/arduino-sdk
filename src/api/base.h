#ifndef BASE_H
#define BASE_H

#include "host/host.h"
#include "http/http.h"
#include "IoTeXConstants.h"

namespace iotex
{
namespace api
{
class Base
{
  protected:
	Host& host_;
	IHTTP* http_;

	template<typename HTTPType>
	explicit Base(Host& host, HTTPType& http) : host_(host), http_(static_cast<IHTTP*>(&http))
	{
	}
};

} // namespace api
} // namespace iotex

#endif
