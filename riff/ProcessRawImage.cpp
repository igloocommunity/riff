/*
 * ProcessRawImage.cpp
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
#include "ProcessRawImage.h"
#include <stdlib.h>
#include <sys/stat.h>
#include "LCDriver.h"
using namespace flash;

ProcessRawImage::ProcessRawImage(const char* path, uint64 start, int useBulk, int deleteBuffers):
		logger_("ProcessRawImage")
{
    pchPath = path;
    uiStart = start;
    uiDevice = 0; // Flashing the flash0 device
    iUseBulk = useBulk;
    iDeleteBuffers = deleteBuffers;
}

int ProcessRawImage::run(DUT* dut)
{
	logger_.log(Logger::PROGRESS, "Flashing raw image...");
    uint64 length = filesize(pchPath);
    int error = 0;

    if (length != 0) {
        error = Flash_FlashRaw(dut->getLCDContext(), pchPath, uiStart, length, uiDevice, iUseBulk, iDeleteBuffers);
        if (0 != error)
        {
        	logger_.log(Logger::ERROR,"LCD ERROR: Process raw image failed %d", error);
            return error;
        }

        // Need a new line braek after the progress bar
        logger_.logRaw("\n");
        logger_.log(Logger::PROGRESS, "Flashing finished successfully");
    } else {
        error = -1;
        logger_.log(Logger::ERROR,"LCD ERROR: Flash image is empty or doesn't exist %d", error);
        return error;
    }

    return error;
}

size_t ProcessRawImage::filesize(const char* filename)
{
    struct stat st;

    if (stat(filename, &st) == 0)
        return st.st_size;

    return 0;
}

const char * ProcessRawImage::get_command_name()
{
    return (char *)"PROCESSRAWIMAGE";
}


/* @} */
