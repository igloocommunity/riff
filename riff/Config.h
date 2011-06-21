/*
 * Config.h
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

#pragma once
#include "Logger.h"
#include <string>
#include <map>
#include <utility>

/**
 * @brief Class used for storing application configuration parameters.
 *
 * This class support reading and storing configuration parameters from/to
 * configuration files. It also supports overriding of configuration parameters
 * from command line.
 */
class Config
{
public:
    /**
     * @brief Default constructor creating empty configuration.
     */
    Config();

    /**
     * @brief Constructor that reads configuration parameters form file.
     *
     * @param[in] path - Path to the configuration file.
     */
    Config(const std::string& path);

    /**
     * @brief Function to load configuration from file.
     *
     * @param[in] path - Path to the configuration file.
     */
    bool load(const std::string& path);

    /**
     * @brief Gets the configuration value as C-style string.
     *
     * @param[in] key - Key of the configuration value.
     * @return Configuration value as C-style string.
     */
    const char* getValue(const std::string& key) const;

    /**
     * @brief Checks if the configuration value exists.
     *
     * @param[in] key - Key of the configuration value.
     * @return True if value exists, otherwise false.
     */
    bool valueExists(const std::string& key) const;

private:
    std::map<std::string, std::string> values_;
    Logger logger_;

    std::string getToken(std::string& line);
};

/* @} */
