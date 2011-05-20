/*
 * CommDeviceManager.cpp
 *
 * Copyright (C) ST-Ericsson SA 2011
 * Authors: Srimanta Panda <srimanta.panda@stericsson.com>,
 *          Ola Borgelin <ola.borgelin@stericsson.com>,
 *          Karin Hedlund <karin.hedlund@stericsson.com>,
 *          Markus Andersson <markus.m.andersson@stericsson.com> for ST-Ericsson.
 * License terms: 3-clause BSD license
 *
 */

#include "CommDeviceManager.h"
#include "CommDevice.h"
#include "CommException.h"
#include "Debug.h"
#include <vector>
using namespace std;

vector<CommDevice*> CommDeviceManager::devices_;

const vector<CommDevice*>& CommDeviceManager::getAllDevices()
{
    return devices_;
}

void CommDeviceManager::destroyDevice(CommDevice* device)
{
    if (0 == device) {
        return;
    }

    vector<CommDevice*>::iterator i = devices_.begin();

    while (i != devices_.end()) {
        if (device == *i) {
            delete device;
            break;
        }

        ++i;
    }

    if (i != devices_.end()) {
        devices_.erase(i);
    }
}

void CommDeviceManager::destroyAll()
{
    for (vector<CommDevice*>::iterator i = devices_.begin(); i != devices_.end(); ++i) {
        delete *i;
    }

    devices_.clear();
}
