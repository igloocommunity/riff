/*
 * DumpArea.h
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
 * @brief Class implementing Dump Area command from Flash group.
 */
class DumpArea : public Command
{
public:
    /**
     * @brief Constructor initializing command parameters.
     *
     * @param[in] path          - Path to the device to dump.
     * @param[in] start         - Start of the dump relative to the start of the device indicated by Path [Byte].
     * @param[in] lenght		- Length of the dump [Byte]. Actual length is determined by the device block size.
     * @param[in] filepath		- File path on PC to store dump data to.
     * @param[in] redundant		- dump with redundant data, 1-> dump without redundant data.
     * @param[in] useBulk       - 1 = use bulk protocol.
     */
	DumpArea(const char* path, uint64 start, uint64 length, const char* filepath, uint32 redundant, int useBulk);

    /**
     * @brief Method that executes the dump process.
     *
     * @param[in] dut - Specific DUT instance on which to execute the command.
     * @return int    - Command execution error (0 on success, non-0 otherwise).
     */
    int run(DUT* dut);

    const char * get_command_name();

private:

    int checkInput();

    Logger logger_;
    const char* pchPath;
    uint64 uiStart;
    uint64 uiLength;
    const char* pchFilePath;
    char dumpPath[50];
	uint32 uiRedundantArea;
    int iUseBulk;
};
}

/* @} */
