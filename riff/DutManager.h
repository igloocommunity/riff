/*
 * DUTManager.h
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
 * @addtogroup DUT Manager
 * @{
 */

#pragma once

#include "DUT.h"
#include "CDAL.h"
#include "Logger.h"
#include <vector>

class DutManager
{
public:
    /**
     * @brief Creates new DUT instance, starts execution and adds the DUT in the list of active devices.
     *
     * @param[in] Device_t - Communication device on which the DUT will be created.
     *
     */
    static DUT* createDut(Device_t device);

    /**
     * @brief Get the DUT instance that is associated with the connected devices..
     *
     * @param[in] Device_t - Communication device.
     * @return    DUT*     - pointer to the DUT.
     * @return    0        - if the DUT can't be find in the list.
     */
    static const DUT* getDut(Device_t device);

    /**
     * @brief Destroys the DUT instance that is associated with the communication device.
     *
     * @param[in] Device_t - Communication device.
     */
    static void destroyDut(Device_t device);

private:
    DutManager();
    ~DutManager();

    /*list of connected devices*/
    static std::vector<DUT*> devices_;

    static std::string getDutId(Device_t device);
};

/* @} */

