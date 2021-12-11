#ifndef IOTEX_RANDOM_H
#define IOTEX_RANDOM_H

#include "stddef.h"
#include "stdint.h"

namespace iotex
{
class Random
{
  public:
	/**
	 * @brief Fill a buffer with random bytes
	 *
	 * @param[out] buf A pointer to the buffer
	 * @param size The buffer size in bytes
	 */
	virtual void fillRandom(uint8_t* buf, size_t size);
};
} // namespace iotex

// Declare global object
extern iotex::Random randomGenerator;

#endif