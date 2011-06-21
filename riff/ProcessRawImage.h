/*
 * ProcessRawImage.h
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
 * @brief Class implementing Process Raw Image command from Flash group.
 */
class ProcessRawImage : public Command
{
public:
    /**
     * @brief Constructor initializing command parameters.
     *
     * @param[in] path          - Path to the image file.
     * @param[in] start         - Address where flashing should start.
     * @param[in] useBulk       - 1 = use bulk protocol.
     * @param[in] deleteBuffers - 1 = release flash archive buffers after finish.
     */
    ProcessRawImage(const char* path, uint64 start, int useBulk, int deleteBuffers);

    /**
     * @brief Method that executes the flashing process.
     *
     * @param[in] dut - Specific DUT instance on which to execute the command.
     * @return int    - Command execution error (0 on success, non-0 otherwise).
     */
    int run(DUT* dut);

    const char * get_command_name();

private:
    Logger logger_;
    uint64 filesize(const char* filename);
    const char* pchPath;
    uint64 uiStart;
    uint32 uiDevice;
    int iUseBulk;
    int iDeleteBuffers;
};
}

/* @} */
