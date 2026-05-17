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
 * - LOG_LEVEL_OVERRIDE
 * When defined will override the LOG_LEVEL to to its level.
 *
 * - How To Use -
 * Add to the start of a cpp file:
 * #define LOG_LEVEL 3
 * #include "logger.h"
 * ================================================================================================
 */
#include <iostream>
// Sets the log level to LOG_LEVEL_OVERRIDE for all files.
#ifdef LOG_LEVEL_OVERRIDE
#undef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_OVERRIDE
#endif
// Sets the log level to 0 for all files.
#ifdef NO_LOG_OVERRIDE
#undef LOG_LEVEL
#define LOG_LEVEL 0
#endif

// ========= Log Helpers ==========================================================================
#define PrintHex(x) "0x" << std::hex << (uint32_t)(x) << std::dec
#define PrintHexPadded(x, width) \
    "0x" << std::hex << std::setw(width) << std::setfill('0') << (uint32_t)(x) << std::dec

// ========= Debug ================================================================================
#if LOG_LEVEL >= 3
// Logs a debug message to standard cout. Need LOG_LEVEL >= 3.
#define LogDebugPrefixed(x, prefix) std::cout << "Debug - " << prefix << ": " << x << std::endl
#define LogDebug(x) std::cout << "Debug: " << x << std::endl
#else
// Logs a debug message to standard cout. Need LOG_LEVEL >= 3.
#define LogDebugPrefixed(x, prefix)
#define LogDebug(x)
#endif

// ========= Info =================================================================================
#if LOG_LEVEL >= 2
// Logs an info message to standard cout. Need LOG_LEVEL >= 2.
#define LogInfoPrefixed(x, prefix) std::cout << "Info - " << prefix << ": " << x << std::endl
#define LogInfo(x) std::cout << "Info: " << x << std::endl
#else
// Logs an info message to standard cout. Need LOG_LEVEL >= 2.
#define LogInfoPrefixed(x, prefix)
#define LogInfo(x)
#endif

// ========= Warning ==============================================================================
#if LOG_LEVEL >= 2
// Logs a warning message to standard cout. Need LOG_LEVEL >= 2.
#define LogWarningPrefixed(x, prefix) std::cout << "Warning - " << prefix << ": " << x << std::endl
#define LogWarning(x) std::cout << "Warning: " << x << std::endl
#else
// Logs a warning message to standard cout. Need LOG_LEVEL >= 2.
#define LogWarningPrefixed(x, prefix)
#define LogWarning(x)
#endif

// ========= Error ================================================================================
#if LOG_LEVEL >= 1
// Logs an error message to standard cerr. Need LOG_LEVEL >= 1.
#define LogErrorPrefixed(x, prefix) std::cout << "Error - " << prefix << ": " << x << std::endl
#define LogError(x) std::cerr << "Error: " << x << std::endl
#else
// Logs an error message to standard cerr. Need LOG_LEVEL >= 1.
#define LogErrorPrefixed(x, prefix)
#define LogError(x)
#endif

// ========= Msg ==================================================================================
#if LOG_LEVEL >= 1
// Logs a message to standard cout with no prefix. Need LOG_LEVEL >= 1.
#define LogMsgPrefixed(x, prefix) std::cout << prefix << ": " << x << std::endl
#define LogMsg(x) std::cout << x << std::endl
#else
// Logs a message to standard cout with no prefix. Need LOG_LEVEL >= 1.
#define LogMsgPrefixed(x, prefix)
#define LogMsg(x)
#endif

#endif
