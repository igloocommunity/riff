/*
 * EraseArea.h
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
#include "DUT.h"
#include "Command.h"

namespace flash
{
/**
 * @brief Class implementing Erase Area command from Flash group.
 */
class EraseArea : public Command
{
public:
    /**
     * @brief Constructor initializing command parameters.
     *
     * @param[in] path          - Path to the device to erase.
     * @param[in] start         - Start of the erase relative to the start of the device.
     * @param[in] length        - 1 = use bulk protocol.
     */
    EraseArea(const char* path, uint64 start, uint64 length);

    /**
     * @brief Method that executes the erase process.
     *
     * @param[in] dut - Specific DUT instance on which to execute the command.
     * @return int    - Command execution error (0 on success, non-0 otherwise).
     */
    int run(DUT* dut);

    const char * get_command_name();
private:
    Logger logger_;
    const char* pchPath;
    uint64 uiStart;
    uint64 uiLength;
};
}

/* @} */
