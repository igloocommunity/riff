/*
 * Shutdown.h
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
 * @addtogroup Commands
 * @{
 */

#pragma once

#include "Command.h"
#include "Logger.h"

namespace System
{
/**
 * @brief Class implementing Shutdown command from System group.
 */
class Shutdown : public Command
{
public:
    /**
     * @brief Default constructor taking no parameters.
     */
    Shutdown();

    /**
     * @brief Method that executes the shutdown of the platform.
     *
     * @param[in] dut - Specific DUT instance on which to execute the command.
     * @return int    - Command execution error (0 on success, non-0 otherwise).
     */
    int run(DUT* dut);

    const char * get_command_name();
private:
    Logger logger_;
};
}

/* @} */
