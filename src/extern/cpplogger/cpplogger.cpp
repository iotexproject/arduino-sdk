/*
----------------------------------------------------------------------
File    : cpplogger.cpp
Date    : 10 Mar 2020
Author  : 
Company : 
Purpose : 
---------------------------END-OF-HEADER------------------------------
*/

/*********************************************************************
*
*       Includes
*
**********************************************************************
*/
#include "cpplogger.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <stdbool.h>

using namespace cpplogger;

/*********************************************************************
*
*       Defines, macros
*
**********************************************************************
*/


/*********************************************************************
*
*       Types
*
**********************************************************************
*/


/*********************************************************************
*
*       Static Variables
*
**********************************************************************
*/


/*********************************************************************
*
*       Public Variables
*
**********************************************************************
*/


/*********************************************************************
*
*       Static function prototypes
*
**********************************************************************
*/
/**
 * level_to_str.
 *
 * \access	static const
 * \param	Loglevel	level	
 * \return	void
 */
static const char* level_to_str(LogLevel level);

static void get_prefix(char* buf, LogLevel level, const char* file, int line);

/*********************************************************************
*
*       Implementation
*
**********************************************************************
*/
Logger::Logger(log_fxn print_impl, LogLevel level, void* ctx)
{
    _print_impl = print_impl;
    _level = level;
    _ctx = ctx;
}

void Logger::set_print_fxn(log_fxn print_impl)
{
    _print_impl = print_impl;
}

/**
 * log_buffer. Logs a hex dump from a buffer with a message
 *
 * \param	Loglevel	level  	
 * \param	uint8_t*   	buffer 	
 * \param	size_t     	size   	
 * \param	char       	*file  	
 * \param	int        	line   	
 * \param	char       	*format	
 * \param	mixed      	...    	
 * \return	void
 */
void Logger::log_buffer(LogLevel level, const std::string &module, uint8_t* buffer, uint32_t size, const char *file, int line)
{
    if (_level >= level)  
    {
        if (_modules.find(module)->second >= level)
        {
            char temp[64]; 
            memset(temp, 0, sizeof(temp));
            for(uint32_t i=0; i<size; i++)
            {
                sprintf(temp+strlen(temp), "%02x ", buffer[i]);
                if( ( (i == (size -1)) || (i%16 == 0) ) && (i != 0)) 
                {
                    // strcpy(temp+strlen(temp), "\r\n");
                    log(level, module, file, line, "%s", temp);
                    memset(temp, 0, sizeof(temp));
                }
            }
        }
    }
}

/**
 * log. Logs a message
 *
 * \param	log_level_t	level  	
 * \param	char       	*file  	
 * \param	int        	line   	
 * \param	char       	*format	
 * \param	mixed      	...    	
 * \return	void
 */
void Logger::log(LogLevel level, const std::string &module, const char *file, int line, const char *format, ...)
{
    if(_level >= level) 
    {
        if (_modules.find(module)->second >= level)
        {
            // Temp buffer to store the string. Messages bigger than this will be trimmed   
            char temp[LOG_BUFFER_SIZE] = {0};
            get_prefix(temp, level, file, line);
            va_list arg;
            va_start(arg, format);
            vsnprintf(temp+strlen(temp), sizeof(temp)-strlen(temp), format, arg);
            va_end(arg);
            _print_impl(temp, _ctx);
        }
    }
};

#if defined(ESP8266) || defined(ESP32)
void Logger::log_flash_string(LogLevel level, const std::string &module, const char *file, int line, const __FlashStringHelper *msg)
{
    if(_level >= level) 
    {
        if (_modules.find(module)->second >= level)
        {
            Serial.println(msg);
        }
    }
}
#endif

void Logger::register_module(const std::string& name, LogLevel level)
{
    _modules.emplace(std::make_pair(name, level));
}

void Logger::set_global_level(LogLevel level)
{
    _level = level;
}

void Logger::set_module_level(const std::string& module, LogLevel level)
{
    _modules.find(module)->second = level;
}

void get_prefix(char* buf, LogLevel level, const char* file, int line)
{
    strcpy(buf, level_to_str(level));
    strcpy(buf+strlen(buf), ": ");
    strcpy(buf+strlen(buf), file);
    strcpy(buf+strlen(buf), ":");
    sprintf(buf+strlen(buf), "%d", line);
    strcpy(buf+strlen(buf), ": ");
}

static const char* level_to_str(LogLevel level) 
{
    switch(level)
    {
        case LogLevel::NONE:
            return "NONE";
        case LogLevel::TRACE:
            return "TRACE";
        case LogLevel::DEBUG:
            return "DEBUG";
        case LogLevel::ERROR:
            return "ERROR";
        case LogLevel::INFO:
            return "INFO";
        case LogLevel::WARNING:
            return "WARNING";
        default:
            return "";
    }
}

log_fxn Logger::get_print_fxn()
{
    return _print_impl;
}

LogLevel Logger::get_level(void)
{
    return _level;
}

/*************************** End of file ****************************/

