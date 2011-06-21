/*
 * Utilities.h
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
 * @addtogroup Utilities
 * @{
 */

#pragma once
#include <sstream>
#include <string>

using namespace std;
class Utilities
{
public:
    static void sleep(int ms) {
        timespec delay;
        delay.tv_sec = 0;
        delay.tv_nsec = ms * 1000000;
        nanosleep(&delay, 0);
    }

    template<typename T, typename F>
    static T convert(const F& fromValue) {
        stringstream stream;
        stream << fromValue;

        T toValue;
        stream >> toValue;
        return toValue;
    }

    static void trim(std::string& s) {
        static const std::string whitespace(" \t\r\n");
        size_t start = s.find_first_not_of(whitespace);

        if (std::string::npos != start) {
            s.erase(0, start);
            size_t end = s.find_last_not_of(whitespace);
            s.erase(end + 1);
        } else {
            s.clear();
        }
    }
};

/* @} */
