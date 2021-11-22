#ifdef ESP32
#include "random/random.h"

#include "esp_system.h"

using namespace iotex;

Random randomGenerator;

void Random::fillRandom(uint8_t* buf, size_t size)
{
	esp_fill_random(buf, size);
}

#endif