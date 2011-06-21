/*
 * Debug.h
 *
 * Copyright (C) ST-Ericsson SA 2011
 * Authors: Srimanta Panda <srimanta.panda@stericsson.com>,
 *          Ola Borgelin <ola.borgelin@stericsson.com>,
 *          Karin Hedlund <karin.hedlund@stericsson.com>,
 *          Markus Andersson <markus.m.andersson@stericsson.com> for ST-Ericsson.
 * License terms: 3-clause BSD license
 *
 */

#pragma once

//#define _DEBUG
#ifdef _DEBUG
#define DEBUG_INFO
#define DEBUG_ERROR
#define DEBUG_HEXDUMP
#define DEBUG_HEXDUMP_SIZE 1024
#endif

#ifdef _WIN32
#define flockfile _lock_file
#define funlockfile _unlock_file
#else
#include <cstddef>
#endif

#if defined(DEBUG_ERROR) || defined(DEBUG_INFO) || defined(DEBUG_HEXDUMP)
#include <cstdio>
#include <cstdarg>
#endif

class Debug
{
public:
    inline static void error(const char* format, ...) __attribute__((format(printf, 1, 0))) {
#ifdef DEBUG_ERROR
        flockfile(stdout);
        printf("ERROR - ");
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
        printf("\n");
        fflush(stdout);
        funlockfile(stdout);
#endif
    }

    static inline void info(const char* format, ...) __attribute__((format(printf, 1, 0))) {
#ifdef DEBUG_INFO
        flockfile(stdout);
        printf("INFO - ");
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
        printf("\n");
        fflush(stdout);
        funlockfile(stdout);
#endif
    }

    inline static void hexdump(const char* message, unsigned char* buffer, size_t length) {
#ifdef DEBUG_HEXDUMP
        flockfile(stdout);
        printf("%s (%d):\n", message, length);
        size_t printLength = length < DEBUG_HEXDUMP_SIZE ? length : DEBUG_HEXDUMP_SIZE;

        for (size_t i = 0; i < printLength; i++) {
            printf("%02x ", buffer[i]);

            if ((i + 1) % 16 == 0 && (i + 1) < printLength)
                printf("\n");
        }

        printf("\n");

        if (printLength != length) {
            printf("%d bytes more...\n", length - printLength);
        }

        printf("\n");
        fflush(stdout);
        funlockfile(stdout);
#endif
    }
private:
    Debug() {}
    ~Debug() {}
    Debug(const Debug&) {}
};
