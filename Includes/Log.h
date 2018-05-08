/** @file Log.h
 * Simple logging system writing to the console.
 * @author Adrien RICCIARDI
 */
#ifndef H_LOG_H
#define H_LOG_H

#include <stdio.h>

//-------------------------------------------------------------------------------------------------
// Constants and macros
//-------------------------------------------------------------------------------------------------
/** Display a red error message on the console.
 * @param stringMessage The message to display.
 * @note Error logs can't be disabled.
 */
#define LOG_ERROR(stringMessage, ...) printf("\033[31m[%s:%s():%d ERROR] " stringMessage "\033[0m\n", __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)

/** @def LOG_DEBUG() Display a green debug message on the console.
 * @param stringMessage The message to display.
 * @note Debug logs are enabled only in debug mode.
 */
#ifndef NDEBUG
	#define LOG_DEBUG(stringMessage, ...) printf("\033[32m[%s:%s():%d DEBUG] " stringMessage "\033[0m\n", __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
	#define LOG_DEBUG(stringMessage, ...) {} // Replace by an empty scope instead of nothing in case the log is the only instruction of an if, which would have an empty body if logs are disabled
#endif

#endif
