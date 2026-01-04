// SPDX-License-Identifier: MIT
/**
 * @file rts_log.h
 * @brief Logging macros with color support.
 */
#ifndef RTS_LOG_H
#define RTS_LOG_H

#include <stdio.h>

/* Console Colors */
#ifndef RTS_NO_COLOR
    #define ANSI_RESET   "\033[0m"
    #define ANSI_RED     "\033[31m"
    #define ANSI_GREEN   "\033[32m"
    #define ANSI_YELLOW  "\033[33m"
    #define ANSI_CYAN    "\033[36m"
    #define ANSI_GRAY    "\033[90m"
#else
    #define ANSI_RESET   ""
    #define ANSI_RED     ""
    #define ANSI_GREEN   ""
    #define ANSI_YELLOW  ""
    #define ANSI_CYAN    ""
    #define ANSI_GRAY    ""
#endif

/**
 * Log Macros
 * Uses ##__VA_ARGS__ to handle cases with no format arguments
 */
#define RTS_LOG_ARRIVAL(fmt, ...) \
    printf(ANSI_CYAN "[ ARRIVAL ] " ANSI_RESET fmt, ##__VA_ARGS__)

#define RTS_LOG_MISS(fmt, ...) \
    printf(ANSI_RED "[ MISS ] " ANSI_RESET fmt, ##__VA_ARGS__)

#define RTS_LOG_MISS_IMMINENT(fmt, ...) \
    printf(ANSI_RED "[ MISS-IMMINENT ] " ANSI_RESET fmt, ##__VA_ARGS__)

#define RTS_LOG_END(fmt, ...) \
    printf(ANSI_GRAY "[ END-BOUNDARY ] " ANSI_RESET fmt, ##__VA_ARGS__)

#define RTS_LOG_DONE(fmt, ...) \
    printf(ANSI_GREEN "    → [DONE] " ANSI_RESET fmt, ##__VA_ARGS__)

#define RTS_LOG_RUN(clock, fmt, ...) \
    printf(ANSI_YELLOW "[ %03d ] " ANSI_RESET fmt, \
           (clock), ##__VA_ARGS__)

/**
 * Debug macro: Can be disabled by not defining RTS_DEBUG 
 */
#ifdef RTS_DEBUG
    #define RTS_LOG_DEBUG(fmt, ...) \
        printf(ANSI_GRAY "[DBUG] " fmt ANSI_RESET, ##__VA_ARGS__)
#else
    #define RTS_LOG_DEBUG(fmt, ...) do {} while(0)
#endif

#endif /* RTS_LOG_H */
