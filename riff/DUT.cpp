/*
 * DUT.cpp
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
 * @addtogroup DUT
 * @{
 */

#include "DUT.h"
#include "CDAL.h"
#include "LCDriver.h"
#include "Config.h"
#include <cstring>
#include <string>
#include <iostream>
using namespace std;

/**
 * @brief Macro used by all command implementations to verify the Command return values.
 */
#define COMMAND_ERROR_VERIFY(error, message) \
    do { \
        if (0 != error) \
        { \
            logger_.log(Logger::ERR,"COMMAND ERROR: %s %d", message, error); \
            usb_destroy_device(commDevice_, error); \
            errorcode_ = error; \
            return; \
        } \
    } while (0);

DUT::DUT(Device_t commDevice, const string& id, CommandSequence_t sequence):
        commDevice_(commDevice),
        id_(id),
        lcdContext_(0),
        executionThread_(0),
        shutdown_(false),
        sequence_(sequence),
        logger_(id)
{

}

DUT::~DUT()
{
    shutdown_ = true;
    executionThread_->wait(1000);

    logger_.log(Logger::INFO, "Destroying LCD context...");
    int error = DestroyContext(&lcdContext_);

    if (0 != error) {
        logger_.log(Logger::ERR, "LCD ERROR: Failed to destroy LCD context %d", error);
    } else {
        logger_.log(Logger::INFO, "LCD context destroyed successfully");
    }

    executionThread_->wait(1000);

    for (CommandSequence_t::iterator i = sequence_.begin(); i != sequence_.end(); ++i) {
        delete *i;
    }

    delete executionThread_;
}

void DUT::startExecution()
{
    executionThread_ = new Thread(ExecutionThreadFunction, this);
}

int DUT::getErrorCode() const
{
    return errorcode_;
}

void DUT::executeSequence()
{
    int error = 0;

    for (CommandSequence_t::iterator i = sequence_.begin(); !shutdown_ && i != sequence_.end(); ++i) {
        error = (*i)->run(this);

    	// When the initialize DUT is failed, just set the error and exit (because
        // the DUT is not started yet). But when other execution is failed, do a
        // smooth exit by shutting down the DUT.
        if ((error != 0) && (!strcmp((*i)->get_command_name(), "INITIALIZE_DUT"))) {
            errorcode_ = error;
        } else if ((error != 0) && (strcmp((*i)->get_command_name(), "SHUTDOWN"))) {
            continue;
        }

        COMMAND_ERROR_VERIFY(error, "Command execution failed");
    }

    return;
}

void* DUT::ExecutionThreadFunction(void* arg)
{
    DUT* dut = static_cast<DUT*>(arg);
    dut->executeSequence();
    return 0;
}

/* @} */
