/*
 * DUT.h
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

#pragma once

#include "CDAL.h"
#include "LCDriver.h"
#include "Thread.h"
#include "Config.h"
#include "Logger.h"
#include "Command.h"
#include "SequenceFactory.h"
#include <string>
#include <vector>

/**
 * @brief Class that wraps all parameters related to the connected device.
 *
 * It also executes the command sequence that is set for this device.
 */
class DUT
{
public:

    /**
     * @brief Constructor.
     *
     * Constructor for the DUT class. It associates the newly created DUT with the
     * communication device and sets the ID of the DUT.
     *
     * @param[in] commDevice - Communication device associated with the DUT.
     * @param[in] id         - ID of the DUT.
     * @param[in] sequence   - Execution sequence for the DUT.
     */
    DUT(Device_t commDevice, const std::string& id, CommandSequence_t sequence);

    /**
     * @brief Destructor.
     *
     * De-initializes the DUT by releasing all the resources used, stopping the execution
     * of command sequence and destroying related LCD context.
     */
    ~DUT();

    /**
     * @brief Start of execution of command sequence.
     *
     * Creates and starts new command execution thread. Each DUT instance is associated
     * with separate command execution thread.
     */
    void startExecution();

    /**
     * @brief This method returns the communication device associated with the DUT.
     *
     * @return Device_t - Communication device associated with the DUT.
     */
    Device_t getCommDevice() const {
        return commDevice_;
    }

    /**
     * @brief This method returns the ID of the DUT.
     *
     * @return const char* - string that contains the ID.
     */
    const char* getId() const {
        return id_.c_str();
    }

    /**
     * @brief This method returns the LCD Context associated with the DUT.
     *
     * @return LCDContext - LCD Context associated with the DUT.
     */
    LCDContext getLCDContext() {
        return lcdContext_;
    }

    /**
     * @brief This method associates the DUT with a new LCDContext.
     *
     * @param[in] LCDContext - LCD Context that will be associated with the DUT.
     */
    void setLCDContext(LCDContext lcdContext) {
        lcdContext_ = lcdContext;
    }

    /**
     * @brief This method returns the errorcode produced by the sequence execution.
     */
    int getErrorCode() const;

private:
    Device_t commDevice_;
    std::string id_;
    LCDContext lcdContext_;
    Logger logger_;
    CommandSequence_t sequence_;
    int errorcode_;

    // command execution thread
    Thread* executionThread_;
    bool shutdown_;
    static void* ExecutionThreadFunction(void* arg);
    void executeSequence();
};

/* @} */
