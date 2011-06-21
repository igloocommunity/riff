/*
 * Logger.h
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

#pragma once
#include <string>

/**
 * @brief Class for storing and displaying logging information.
 *
 * This class implements simple command line logger that can display logging
 * information either to the application console or separate logging console.
 */
class Logger
{
public:

    enum LogSeverity {
        PROGRESS,
        ERROR,
        WARNING,
        INFO
    };


    /**
     * @brief Creates logger with the specified name.
     *
     * @param[in] name       - Name of the logger.
     */
    Logger(const std::string& name);

    /**
     * @brief Destroys the logger.
     */
    ~Logger();

    /**
     * @brief printf style log function to print formated message.
     *
     * It prints the message on separate line adding timestamp and logger name
     * if printing in a shared console.
     *
     * @param[in] severity  - the severity of the log message.
     * @param[in] format - printf style format of the message.
     * @param[in] ...    - variable argument list.
     */
    void log(LogSeverity severity, const char* format, ...) const __attribute__((format(printf, 3, 0)));

    /**
     * @brief Print hex dump of the buffer.
     *
     * @param[in] buffer - buffer to be printed to the log.
     * @param[in] size   - size of the buffer.
     */
    void hex(const unsigned char* buffer, size_t size) const;

    /**
     * @brief Print raw message string to the logging console.
     *
     * @param[in] message - raw messsage to be printed.
     */
    void logRaw(const char* message) const;

    static bool verbose_;

private:
    FILE* console_;
    std::string name_;
};

/* @} */
