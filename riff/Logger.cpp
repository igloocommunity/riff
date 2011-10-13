/*
 * Logger.cpp
 *
 * Copyright (C) ST-Ericsson SA 2011
 * Authors: Srimanta Panda <srimanta.panda@stericsson.com>,
 *          Ola Borgelin <ola.borgelin@stericsson.com>,
 *          Karin Hedlund <karin.hedlund@stericsson.com>,
 *          Markus Andersson <markus.m.andersson@stericsson.com> for ST-Ericsson.
 * License terms: 3-clause BSD license
 *
 */

/*
 * @addtogroup Logger
 * @{
 */


#include "Logger.h"
#include <string>
#include <cstdio>
#include <ctime>
#include <cstdarg>
#include <cstddef>
using namespace std;

#ifdef _WIN32
#define flockfile _lock_file
#define funlockfile _unlock_file
#define popen _popen
#define pclose _pclose
#define localtime_r(t, lt) localtime_s(lt, t)
#endif

bool Logger::verbose_;

Logger::Logger(const string& name):
        name_(name)
{
    console_ = stdout;

}

Logger::~Logger()
{
}

void Logger::log(LogSeverity severity, const char* format, ...) const
{
    time_t t = time(0);
    tm lt;
    localtime_r(&t, &lt);

    if (verbose_ || severity == Logger::PROGRESS || severity == Logger::ERR) {
        flockfile(console_);
        fprintf(console_, "%02d:%02d:%02d ", lt.tm_hour, lt.tm_min, lt.tm_sec);

        fprintf(console_, " - ");

        va_list args;
        va_start(args, format);
        vfprintf(console_, format, args);
        va_end(args);
        fprintf(console_, "\n");
        fflush(console_);
        funlockfile(console_);

    }
}

void Logger::hex(const unsigned char* buffer, size_t size) const
{
    flockfile(console_);

    for (size_t i = 0; i < size; i++) {
        fprintf(console_, "%02x ", buffer[i]);

        if ((i + 1) % 16 == 0 || (i + 1) == size)
            fprintf(console_, "\n");
    }

    fflush(console_);
    funlockfile(console_);
}

void Logger::logRaw(const char* message) const
{
    flockfile(console_);
    fprintf(console_, "%s", message);
    fflush(console_);
    funlockfile(console_);
}

/* @} */
