#ifndef CLIENT_HELPERS_H
#define CLIENT_HELPERS_H

#include "extern/cpplogger/cpplogger.h"
#include "IoTeXResultCodes.h"
#include <cstring>
#include <string>
#include <vector>

// clang-format off

/**************************************************************************/
/* Log level macros */
/**************************************************************************/

#define IOTEX_LOG_LEVEL_TRACE cpplogger::LogLevel::TRACE
#define IOTEX_LOG_LEVEL_DEBUG cpplogger::LogLevel::DEBUG
#define IOTEX_LOG_LEVEL_INFO cpplogger::LogLevel::INFO
#define IOTEX_LOG_LEVEL_WARNING cpplogger::LogLevel::WARNING
#define IOTEX_LOG_LEVEL_ERROR cpplogger::LogLevel::ERROR
#define IOTEX_LOG_LEVEL_NONE cpplogger::LogLevel::NONE

#define IOTEX_DEFAULT_LOG_LEVEL IOTEX_LOG_LEVEL_NONE

/**************************************************************************/
/* Set default log level */
/**************************************************************************/

#ifndef IOTEX_LOG_LEVEL
	#define IOTEX_LOG_LEVEL IOTEX_DEFAULT_LOG_LEVEL
#endif

/**************************************************************************/
/* Conditinally include board specific files and add board specific flags */
/**************************************************************************/

#if defined(ARDUINO) || defined(ESP8266) || defined(ESP32) || defined(__SAMD21G18A__)
	#include <Arduino.h>
	#define IotexString String	// Use Arduino String intead of std::string in Arduino
#else
	#define IotexString std::string
	#ifndef OS
		#define OS
	#endif
#endif

/**************************************************************************/
/* Remove log statements from compiled binary if IOTEX_ENABLE_LOG not defined */
/**************************************************************************/

#ifndef IOTEX_ENABLE_LOG
	#define IOTEX_DEBUG(module, ...)
	#define IOTEX_INFO(module, ...) 
	#define IOTEX_WARNING(module, ...) 
	#define IOTEX_TRACE(module, ...) 
	#define IOTEX_ERROR(module, ...)
	#define IOTEX_DEBUG_F(module, ...)
	#define IOTEX_INFO_F(module, ...) 
	#define IOTEX_WARNING_F(module, ...) 
	#define IOTEX_TRACE_F(module, ...) 
	#define IOTEX_ERROR_F(module, ...)
	#define IOTEX_DEBUG_BUF(module, ...)
	#define IOTEX_INFO_BUF(module, ...) 
	#define IOTEX_WARNING_BUF(module, ...) 
	#define IOTEX_TRACE_BUF(module, ...) 
	#define IOTEX_ERROR_BUF(module, ...)
#endif

/**************************************************************************/
/* Helper macros for logging a message with different levels */
/**************************************************************************/

#ifdef IOTEX_ENABLE_LOG
	#define IOTEX_DEBUG(module, ...) IotexHelpers.logger.LOG_MSG(cpplogger::LogLevel::DEBUG, module, __VA_ARGS__)
	#define IOTEX_INFO(module, ...) IotexHelpers.logger.LOG_MSG(cpplogger::LogLevel::INFO, module, __VA_ARGS__)
	#define IOTEX_WARNING(module, ...) IotexHelpers.logger.LOG_MSG(cpplogger::LogLevel::WARNING, module, __VA_ARGS__)
	#define IOTEX_TRACE(module, ...) IotexHelpers.logger.LOG_MSG(cpplogger::LogLevel::TRACE, module, __VA_ARGS__)
	#define IOTEX_ERROR(module, ...) IotexHelpers.logger.LOG_MSG(cpplogger::LogLevel::ERROR, module, __VA_ARGS__)
#endif

/**************************************************************************/
/* Helper macros for logging a hex dump of a buffer with different levels */
/**************************************************************************/

#ifdef IOTEX_ENABLE_LOG
	#define IOTEX_DEBUG_BUF(module, buf, size) IotexHelpers.logger.LOG_BUF(cpplogger::LogLevel::DEBUG, module, buf, size)
	#define IOTEX_INFO_BUF(module, buf, size) IotexHelpers.logger.LOG_BUF(cpplogger::LogLevel::INFO, module, buf, size)
	#define IOTEX_WARNING_BUF(module, buf, size) IotexHelpers.logger.LOG_BUF(cpplogger::LogLevel::WARNING, module, buf, size)
	#define IOTEX_TRACE_BUF(module, buf, size) IotexHelpers.logger.LOG_BUF(cpplogger::LogLevel::TRACE, module, buf, size)
	#define IOTEX_ERROR_BUF(module, buf, size) IotexHelpers.logger.LOG_BUF(cpplogger::LogLevel::ERROR, module, buf, size)
#endif

/**************************************************************************/
/* Helper macros for logging a flash string with different levels (for ESP) */
/**************************************************************************/

#ifdef IOTEX_ENABLE_LOG
	#if !defined(ESP8266) && !defined(ESP32)
		#define IOTEX_FLASH_STRING_SUPPORT false
	#else
		#define IOTEX_FLASH_STRING_SUPPORT true
	#endif

	#if !IOTEX_FLASH_STRING_SUPPORT
		#define IOTEX_DEBUG_F(module, ...) IOTEX_DEBUG(module, __VA_ARGS__)
		#define IOTEX_INFO_F(module, ...) IOTEX_INFO(module, __VA_ARGS__)
		#define IOTEX_WARNING_F(module, ...) IOTEX_WARNING(module, __VA_ARGS__)
		#define IOTEX_TRACE_F(module, ...) IOTEX_TRACE(module, __VA_ARGS__)
		#define IOTEX_ERROR_F(module, ...) IOTEX_ERROR(module, __VA_ARGS__)
	#else
		#define IOTEX_DEBUG_F(module, msg) IotexHelpers.logger.LOG_PROGMEM_STRING(cpplogger::LogLevel::DEBUG, module, msg)
		#define IOTEX_INFO_F(module, msg) IotexHelpers.logger.LOG_PROGMEM_STRING(cpplogger::LogLevel::DEBUG, module, msg)
		#define IOTEX_WARNING_F(module, msg) IotexHelpers.logger.LOG_PROGMEM_STRING(cpplogger::LogLevel::DEBUG, module, msg)
		#define IOTEX_TRACE_F(module, msg) IotexHelpers.logger.LOG_PROGMEM_STRING(cpplogger::LogLevel::DEBUG, module, msg)
		#define IOTEX_ERROR_F(module, msg) IotexHelpers.logger.LOG_PROGMEM_STRING(cpplogger::LogLevel::DEBUG, module, msg)
	#endif
#endif

// clang-format on

namespace iotex
{
/**
 * @brief Enum that contains the different logging modules of the library
 *
 */
enum LogModules
{
	USER,
	GENERAL,
	HTTP,
	CONTRACT,
	VALUES_COUNT
};

/**
 * @brief General log module name exposed to the user, so the logger can be used by user code if
 * needed
 *
 */
extern const std::string& userLogModule;

/**
 * @brief Lookup table for log module names. This should be only used by the internal library code
 *
 */
extern const std::string logModuleNamesLookupTable[(int)LogModules::VALUES_COUNT];

/**
 * @brief Enum representing all the log levels
 *
 */
enum IotexLogLevel
{
	NONE,
	ERROR,
	WARNING,
	INFO,
	DEBUG,
	TRACE
};

/**
 * @brief Class that contains helper methods
 *
 */
class Helpers
{
  public:
	Helpers();
	cpplogger::Logger logger;

	/**
	 * @brief Gets the hex string representation of a vector of bytes
	 *
	 * @param data
	 * @param[out] out
	 */
	void vectorToHexString(std::vector<uint8_t>& data, IotexString& out);

	/**
	 * @brief Gets the result strinf value of a ResultCode
	 *
	 * @param code
	 * @return[out] const char*
	 */
	const char* GetResultString(ResultCode code);

	/**
	 * @brief Sets the log level globally. Overrides log level modules if they are of a higher level
	 *
	 * @param level
	 */
	void setGlobalLogLevel(IotexLogLevel level);

	/**
	 * @brief Set the Mlog level for a module. Note that the global lov level can override a module
	 * log level
	 *
	 * @param module
	 * @param level
	 */
	void setModuleLogLevel(const std::string& module, IotexLogLevel level);
};
} // namespace iotex

/**
 * @brief IotexHelpers global object
 *
 */
extern iotex::Helpers IotexHelpers;

#endif
