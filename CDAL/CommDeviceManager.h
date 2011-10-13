/*
 * CommDeviceManager.h
 *
 * Copyright (C) ST-Ericsson SA 2011
 * Authors: Srimanta Panda <srimanta.panda@stericsson.com>,
 *          Ola Borgelin <ola.borgelin@stericsson.com>,
 *          Karin Hedlund <karin.hedlund@stericsson.com>,
 *          Markus Andersson <markus.m.andersson@stericsson.com> for ST-Ericsson.
 * License terms: 3-clause BSD license
 *
 */

#pragma once

#include "CDAL.h"
#include "CommDevice.h"
#include <vector>
#ifdef _WIN32
#include <usb.h>
#else
#include <libusb.h>
#endif

class CommDeviceManager
{
public:
    static const std::vector<CommDevice*>& getAllDevices();

    static void destroyDevice(CommDevice* device);
    static void destroyAll();

    template <class T, class U>
    static T* createDevice(U port) {
        T* device = new T(port);

        T* existingDevice = dynamic_cast<T*>(getDevice<T>(port));

        if (0 != existingDevice) {
            destroyDevice(existingDevice);
        }

        devices_.push_back(device);

        return device;
    }

    template <class T, class U>
    static T* getDevice(U port) {
        T* device;

        for (std::vector<CommDevice*>::iterator i = devices_.begin(); i != devices_.end(); ++i) {
            device = dynamic_cast<T*>(*i);

            if (0 != device && device->getPort() == port) {
                return device;
            }
        }

        return 0;
    }


private:
    static std::vector<CommDevice*> devices_;
private:
    CommDeviceManager() {}
    CommDeviceManager(const CommDeviceManager&) {}
    ~CommDeviceManager() {}
};

// template specialization used to get device for LCD object mapping
template <>
inline CommDevice* CommDeviceManager::getDevice<CommDevice, void*>(void* object)
{
    for (std::vector<CommDevice*>::iterator i = devices_.begin(); i != devices_.end(); ++i) {
        if ((*i)->getObject() == object) {
            return *i;
        }
    }

    return 0;
}


