/*
 * Command.h
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
 * @addtogroup Command
 * @{
 */

#pragma once

// DUT class declaration
class DUT;

/**
 * @brief Abstract class providing base for all command implementations.
 */
class Command
{
public:
    /**
     * @brief Pure virtual method implemented by concrete command implementation.
     *
     * This method should be implemented in the inherited command class providing
     * specific command behavior.
     *
     * @param[in] dut - Specific DUT instance on which to execute the command.
     * @return int   - Command execution error (0 on success, non-0 otherwise).
     */
    virtual int run(DUT* dut) = 0;

    /**
     * @brief Default virtual destructor.
     */
    virtual ~Command() {}

    /**
     * @brief Get the command name
     */
    virtual const char * get_command_name() = 0;

};

/* @} */
