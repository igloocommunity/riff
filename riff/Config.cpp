/*
 * Config.cpp
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
 * @addtogroup Config
 * @{
 */


#include "Config.h"
#include "Utilities.h"
#include <fstream>
#include <algorithm>
#include <cctype>
using namespace std;

Config::Config(): logger_("Config")
{
}

Config::Config(const string& path): logger_("Config")
{
    load(path);
}

bool Config::load(const string& path)
{
    ifstream file;
    file.open(path.c_str());

    if (file.is_open()) {

        string line;
        string command;
        string key;
        string value;

        while (getline(file, line)) {
            Utilities::trim(line);

            if (line.length() == 0 || line[0] != '!')
                continue;

            line.erase(0, 1);

            command = getToken(line);

            if (command == "set") {
                key = getToken(line);
                values_[key] = line;
            }
        }

        return true;
    } else {
        logger_.log(Logger::ERR, "Failed to open configuration file \"%s\"", path.c_str());
        return false;
    }
}

const char* Config::getValue(const string& key) const
{
    map<string, string>::const_iterator i = values_.find(key);

    if (i != values_.end()) {
        return i->second.c_str();
    } else {
        return "";
    }
}

std::string Config::getToken(std::string& line)
{
    size_t pos = line.find_first_of(" ");
    string token = line.substr(0, pos);
    line.erase(0, pos + 1);
    Utilities::trim(token);
    return token;
}

bool Config::valueExists(const string& key) const
{
    string value = getValue(key);
    return value.size() != 0;
}

/* @} */
