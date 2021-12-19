/*
----------------------------------------------------------------------
File    : cpplogger.h
Date    : 10 Mar 2020
Author  :
Company : 
Purpose : 
---------------------------END-OF-HEADER------------------------------
*/

#ifndef CPP_LOGGER_H
#define CPP_LOGGER_H

/*********************************************************************
*
*       Includes
*
**********************************************************************
*/
#include <stdint.h>
#include <string.h>
#include <vector>
#include <string>
#include <map>

#if defined(ESP8266) || defined(ESP32)
    #include "Arduino.h"
#endif

/*********************************************************************
*
*       Defines, macros
*
**********************************************************************
*/

#ifndef __FILENAME__
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

#define LOG_MSG(level, module, ...) log(level, module, __FILENAME__, __LINE__, __VA_ARGS__)  
#define LOG_BUF(level, module, buffer, size) log_buffer(level, module, buffer, size, __FILENAME__, __LINE__)  
#define LOG_PROGMEM_STRING(level, module, msg) log_flash_string(level, module, __FILENAME__, __LINE__, F(msg))  

#define LOG_BUFFER_SIZE 1024

/*********************************************************************
*
*       Types
*
**********************************************************************
*/

namespace cpplogger
{
    typedef uint32_t (*log_fxn) ( const char *message, void* ctx );

    /**
     * \enum		log_level_t	Enum with the log levels
     */
    enum class LogLevel
    {
        NONE,
        ERROR,
        WARNING,
        INFO,
        DEBUG,
        TRACE
    };

    class Logger
    {
        public: 
            Logger(log_fxn print_impl, LogLevel level, void* ctx = nullptr);

            void register_module(const std::string& name, LogLevel level);
            void log(LogLevel level, const std::string &module, const char *file, int line, const char *format, ...);
            void log_buffer(LogLevel level, const std::string &module, uint8_t* buffer, uint32_t size, const char *file, int line);
            #if defined(ESP8266) || defined(ESP32)
            void log_flash_string(LogLevel level, const std::string &module, const char *file, int line, const __FlashStringHelper *msg);
            #endif
            void set_global_level(LogLevel level);
            void set_module_level(const std::string& module, LogLevel level);
            void set_print_fxn(log_fxn print_impl);

            /** Functions below should only be used for testing **/
            log_fxn get_print_fxn(void);
            LogLevel get_level(void);
        
        private:
            log_fxn _print_impl;
            void* _ctx;
            LogLevel _level;
            std::map<std::string, LogLevel> _modules;
    };
}

/*********************************************************************
*
*       Global variables
*
**********************************************************************
*/

/*********************************************************************
*
*       Functions
*
**********************************************************************
*/

#endif /* #ifndef CPP_LOGGER_H */
/*************************** End of file ****************************/
