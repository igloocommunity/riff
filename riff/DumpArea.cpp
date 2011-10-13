/*
 * DumpArea.cpp
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

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "DumpArea.h"
#include <stdlib.h>
#include <cstring>
#include "LCDriver.h"
using namespace flash;

DumpArea::DumpArea(const char* path, uint64 start, uint64 length, const char* filepath, uint32 redundant, int useBulk):
		logger_("DumpArea")
{
    pchPath = path;
    uiStart = start;
    uiLength = length;
    pchFilePath = filepath;
    uiRedundantArea = redundant;
    iUseBulk = useBulk;;
}

int DumpArea::run(DUT* dut)
{
    logger_.log(Logger::PROGRESS, "Dumping area...");
    logger_.log(Logger::PROGRESS, "Dump path is %s", pchFilePath);
    int error = checkInput();
    if (error == -1) {
       return error;
    }
    error = Flash_DumpArea(dut->getLCDContext(), pchPath, uiStart, uiLength, pchFilePath, uiRedundantArea, iUseBulk);
    if (0 != error) {
       logger_.log(Logger::ERR,"LCD ERROR: Dump area failed %d", error);
       return error;
    }
    // Need a new line braek after the progress bar
    logger_.logRaw("\n");
    logger_.log(Logger::INFO, "Dump area finished successfully");
    return error;
}

int DumpArea::checkInput()
{
    if (uiLength <= 0) {
       logger_.log(Logger::ERR, "Length of the dump not specified. Use -l or --length option");
       return -1;
    }
    return 0;
}

const char * DumpArea::get_command_name()
{
    return (char *)"DUMPAREA";
}


/* @} */
