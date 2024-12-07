#ifndef LOGGER_H
#define LOGGER_H
/**================================================================================================
 *
 * Logger Marcos - Helper macros for outputting messages to the terminal
 *
 * - LOG_LEVEL -
 * Determines which marcos actually expand to cout / cerr statements
 * 0 - No messages
 * 1 - Error / Forced messages via LogMsg()
 * 2 - Info / Warning messages
 * 3 - Debug messages.
 *
 * - NO_LOG_OVERRIDE -
 * When defined will override the LOG_LEVEL to 0.
 *
 * - How To Use -
 * Add to the start of a cpp file:
 * #define LOG_LEVEL 3
 * #include "logger.h"
 * ================================================================================================
 */
#include <iostream>
// Sets the log level to 0 for all files.
#ifdef NO_LOG_OVERRIDE
#define LOG_LEVEL 0
#endif

// ========= Debug
// ================================================================================
#if LOG_LEVEL >= 3
// Logs a debug message to standard cout. Need LOG_LEVEL >= 3.
#define LogDebug(x) std::cout << "Debug: " << x << std::endl
#else
// Logs a debug message to standard cout. Need LOG_LEVEL >= 3.
#define LogDebug(x)
#endif

// ========= Info
// =================================================================================
#if LOG_LEVEL >= 2
// Logs an info message to standard cout. Need LOG_LEVEL >= 2.
#define LogInfo(x) std::cout << "Info: " << x << std::endl
#else
// Logs an info message to standard cout. Need LOG_LEVEL >= 2.
#define LogInfo(x)
#endif

// ========= Warning
// ==============================================================================
#if LOG_LEVEL >= 2
// Logs a warning message to standard cout. Need LOG_LEVEL >= 2.
#define LogWarning(x) std::cout << "Warning: " << x << std::endl
#else
// Logs a warning message to standard cout. Need LOG_LEVEL >= 2.
#define LogWarning(x)
#endif

// ========= Error
// ================================================================================
#if LOG_LEVEL >= 1
// Logs an error message to standard cerr. Need LOG_LEVEL >= 1.
#define LogError(x) std::cerr << "Error: " << x << std::endl
#else
// Logs an error message to standard cerr. Need LOG_LEVEL >= 1.
#define LogError(x)
#endif

// ========= Msg
// ==================================================================================
#if LOG_LEVEL >= 1
// Logs a message to standard cout with no prefix. Need LOG_LEVEL >= 1.
#define LogMsg(x) std::cout << x << std::endl
#else
// Logs a message to standard cout with no prefix. Need LOG_LEVEL >= 1.
#define LogMsg(x)
#endif
#endif