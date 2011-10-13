/*
 * Shutdown.cpp
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

#include "Shutdown.h"
#include "DUT.h"
using namespace System;

Shutdown::Shutdown():
		logger_("ShutDown")
{

}

int Shutdown::run(DUT* dut)
{
	logger_.log(Logger::INFO, "Shutting down device...");
    int error = System_Shutdown(dut->getLCDContext());
    if (0 != error)
    {
        logger_.log(Logger::ERR,"LCD ERROR: Device shutdown failed %d", error);
        return error;
    }

    logger_.log(Logger::INFO, "Device shutdown finished successfully");
    return error;
}

const char * Shutdown::get_command_name()
{
    return (char *)"SHUTDOWN";
}

/* @} */
