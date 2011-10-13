/*
 * EraseArea.cpp
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

#include "EraseArea.h"
using namespace flash;


EraseArea::EraseArea(const char* path, uint64 start, uint64 length):
		logger_("EraseArea")
{
    pchPath = path;
    uiStart = start;
    uiLength = length;
}

int EraseArea::run(DUT* dut)
{
    logger_.log(Logger::PROGRESS, "Erasing area...");
    int error = Flash_EraseArea(dut->getLCDContext(), pchPath, uiStart, uiLength);

    if (0 != error)
    {
        logger_.log(Logger::ERR,"LCD ERROR: Erase area failed %d", error);
        return error;
    }

    logger_.log(Logger::PROGRESS, "Erase area finished successfully");
    return error;
}

const char * EraseArea::get_command_name()
{
    return (char *)"ERASEAREA";
}

/* @} */
