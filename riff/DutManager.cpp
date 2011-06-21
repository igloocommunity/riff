/*
 * DUTManager.cpp
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

#include "DutManager.h"
#include "Utilities.h"
#include <vector>
using namespace std;

vector<DUT*> DutManager::devices_;

DUT* DutManager::createDut(Device_t device)
{
    string dutId = getDutId(device);

    DUT* dut = new DUT(device, dutId, SequenceFactory::getProfileSequence());
    devices_.push_back(dut);

    dut->startExecution();

    return dut;
}

const DUT* DutManager::getDut(Device_t device)
{
    for (vector<DUT*>::iterator i = devices_.begin(); i != devices_.end(); ++i) {
        if ((*i)->getCommDevice() == device) {
            return *i;
        }
    }

    return 0;
}

void DutManager::destroyDut(Device_t device)
{
    for (vector<DUT*>::iterator i = devices_.begin(); i != devices_.end(); ++i) {
        if ((*i)->getCommDevice() == device) {
            delete *i;
            devices_.erase(i);
            break;
        }
    }
}

string DutManager::getDutId(Device_t device)
{
    string physicalId = "Device@" + Utilities::convert<string>(comm_get_physical_address(device));
    return physicalId;
}

/* @} */
