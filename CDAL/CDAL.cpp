/*
 * CDAL.cpp
 *
 * Copyright (C) ST-Ericsson SA 2011
 * Authors: Srimanta Panda <srimanta.panda@stericsson.com>,
 *          Ola Borgelin <ola.borgelin@stericsson.com>,
 *          Karin Hedlund <karin.hedlund@stericsson.com>,
 *          Markus Andersson <markus.m.andersson@stericsson.com> for ST-Ericsson.
 * License terms: 3-clause BSD license
 *
 */

#include "stdafx.h"
#include "CDAL.h"
#include "Debug.h"
#include "CommDevice.h"
#include "CommDeviceManager.h"
#include "CommException.h"
#include "Event.h"
#include "Thread.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include "LibusbDevice_win.h"
#else
#include "LibusbDevice.h"
#endif
using namespace std;

#ifndef _WIN32
static libusb_context* context;
#endif

static int DUT_VENDOR_ID = 0x04cc;
static int DUT_PRODUCT_ID = 0x8500;
static Thread* workerThread;
static void* LibusbWorker(void* arg);
volatile bool shutdown = false;
int error = 0;


static EventCallback_t OnDeviceCallback = NULL;

void usb_init_driver(const char* vendorId, const char* productId)
{
    if (vendorId != NULL && *vendorId != '\0') {
        DUT_VENDOR_ID = strtol(vendorId, NULL, 16);
    }

    if (productId != NULL && *productId != '\0') {
        DUT_PRODUCT_ID = strtol(productId, NULL, 16);
    }

#ifdef _WIN32
    usb_init();
    usb_set_debug(2);
#else
    libusb_init(&context);
    libusb_set_debug(context, 2);
#endif

    workerThread = new Thread(LibusbWorker, 0);
}

void usb_set_listen_callback(EventCallback_t callback)
{
    OnDeviceCallback = callback;
}

void usb_deinit_driver()
{
    shutdown = true;
    workerThread->wait();
    delete workerThread;

    CommDeviceManager::destroyAll();

#ifndef _WIN32
    libusb_exit(context);
#endif

}

void usb_destroy_device(Device_t device, int error_code)
{
    device->cancel();
    error = error_code;
}

int comm_read(Device_t device, void* buffer, size_t size)
{
    return device->read(buffer, size);
}

int comm_read_nowait(void* buffer, size_t size, DataCallback_t cb, void* param)
{
    CommDevice* device = CommDeviceManager::getDevice<CommDevice>(param);

    if (NULL != device) {
        return device->readNoWait(buffer, size, cb, param);
    } else {
        return COMM_DEVICE_OBJECT_NOT_MAPPED;
    }
}

int comm_write(Device_t device, void* buffer, size_t size)
{
    return device->write(buffer, size);
}

int comm_write_nowait(void* buffer, size_t size, DataCallback_t cb, void* param)
{
    CommDevice* device = CommDeviceManager::getDevice<CommDevice>(param);

    if (NULL != device) {
        return device->writeNoWait(buffer, size, cb, param);
    } else {
        return COMM_DEVICE_OBJECT_NOT_MAPPED;
    }
}

int comm_cancel(void* param)
{
    CommDevice* device = CommDeviceManager::getDevice<CommDevice>(param);

    if (NULL != device) {
        return device->cancel();
    } else {
        return COMM_DEVICE_OBJECT_NOT_MAPPED;
    }
}

unsigned long comm_get_physical_address(Device_t device)
{
    return device->getPhysicalAddress();
}

void** comm_get_object_storage(Device_t device)
{
    return device->getObjectStorage();
}

void comm_progress(void *Communication_p, unsigned long long totalbytes,
                   unsigned long long tranferedbytes)
{
    double precentage = (tranferedbytes * 100) / totalbytes;

    printf("\r%.1f %% ", precentage);
    fflush(stdout);
}

#ifdef _WIN32
// As we are going to use libusb 0.1 version in Windows,
// this function needs to be re-writtten with the API
// related to the 0.1 version. The libusb 1.0 and 0.1
// differs a lot according to their function calls and
// the way USB devices are recognized and handled.
static void* LibusbWorker(void* arg)
{
    struct usb_bus *busses;
    struct usb_bus *bus;

    while (!shutdown) {

        usb_find_busses();
        usb_find_devices();
        busses = usb_get_busses();

        if(busses == NULL ) {
            Debug::info("No USB device connected.");
            Sleep(10);
            continue;
        }

        for (bus = busses; bus; bus = bus->next) {
            struct usb_device *dev = NULL;

            for (dev = bus->devices; dev; dev = dev->next) {
                if ((dev->descriptor.idProduct == DUT_PRODUCT_ID) && (dev->descriptor.idVendor == DUT_VENDOR_ID) && (!shutdown)) {

                LibusbDevice *device;

                device = CommDeviceManager::getDevice<LibusbDevice>(dev);

                if (0 == device) {
                    // new device found
                    try {
                        device = CommDeviceManager::createDevice <LibusbDevice> (dev);
                        Debug::info("Libusb worker: Connected libusb device");
                        OnDeviceCallback(COMM_DEVICE_SUCCESS,
                                         LIBUSB_DEVICE_CONNECTED, device);
                    } catch (CommException e) {
                        Debug::error("Libusb worker: %s", e.what());
                        OnDeviceCallback(e.getError(),
                                         COMM_DEVICE_UNDEFINED_EVENT, 0);
                    }
                 }
              }
           }
        }

        vector<CommDevice*> devices = CommDeviceManager::getAllDevices();

        for (vector<CommDevice*>::iterator i = devices.begin(); i
                != devices.end(); ++i) {
            LibusbDevice* device = dynamic_cast<LibusbDevice*>(*i);

            if (0 == device) {
                continue;
            }

            bool connected = false;
            struct usb_device *dev;
            for (bus = busses; bus; bus = bus->next) {
                for (dev = bus->devices; dev; dev = dev->next) {
                    if (dev == device->getPort()) {
                        connected = true;
                        break;
                    }
                }
                if(connected == true)
                   break;
            }

            if (!connected) {
                Debug::info("Libusb worker: Disconnected device with id %d", *i);
                OnDeviceCallback(COMM_DEVICE_SUCCESS,
                                 LIBUSB_DEVICE_DISCONNECTED, *i);
                shutdown = true;
            }
        }

        Sleep(10);
    }

    return 0;
}
#else
// In Linux, libusb 1.0 version is used. Therefore,
// the below LibusbWorker function is based on API
// for libusb 1.0 for handling USB devices.
static void* LibusbWorker(void* arg __attribute__((unused)))
{
    timespec delay;
    delay.tv_sec = 0;
    delay.tv_nsec = 10 * 1000000; // 10ms

    libusb_device** deviceList;
    ssize_t deviceCount;
    libusb_device_descriptor descriptor;

    while (!shutdown) {
        deviceCount = libusb_get_device_list(context, &deviceList);

        if (deviceCount < 0) {
            nanosleep(&delay, 0);
            continue;
        }

        for (int i = 0; i != deviceCount; ++i) {
            int status = libusb_get_device_descriptor(deviceList[i],
                         &descriptor);

            if (status != LIBUSB_SUCCESS) {
                Debug::error(
                    "Libusb worker: error while getting device descriptor for device %d from %d devices",
                    i, deviceCount);
                continue;
            }

            if (descriptor.idVendor == DUT_VENDOR_ID && descriptor.idProduct
                    == DUT_PRODUCT_ID && (!shutdown)) {
                LibusbDevice* device = CommDeviceManager::getDevice <
                                       LibusbDevice, libusb_device* > (deviceList[i]);

                if (0 == device) {
                    // new device found
                    try {
                        device = CommDeviceManager::createDevice < LibusbDevice,
                                 libusb_device* > (deviceList[i]);
                        Debug::info("Libusb worker: Connected libusb device");
                        OnDeviceCallback(COMM_DEVICE_SUCCESS,
                                         LIBUSB_DEVICE_CONNECTED, device);
                    } catch (CommException e) {
                        Debug::error("Libusb worker: %s", e.what());
                        OnDeviceCallback(e.getError(),
                                         COMM_DEVICE_UNDEFINED_EVENT, 0);
                    }
                }
            }
        }

        vector<CommDevice*> devices = CommDeviceManager::getAllDevices();

        for (vector<CommDevice*>::iterator i = devices.begin(); i
                != devices.end(); ++i) {
            LibusbDevice* device = dynamic_cast<LibusbDevice*>(*i);

            if (0 == device) {
                continue;
            }

            bool connected = false;

            for (int j = 0; j != deviceCount; ++j) {
                if (deviceList[j] == device->getPort()) {
                    if (error == 0)
                        connected = true;

                    break;
                }
            }

            if (!connected) {
                Debug::info("Libusb worker: Disconnected device with id %d", *i);
                OnDeviceCallback(COMM_DEVICE_SUCCESS,
                                 LIBUSB_DEVICE_DISCONNECTED, *i);
                shutdown = true;
            }
        }

        libusb_free_device_list(deviceList, 1);

        nanosleep(&delay, 0);
    }

    return 0;
}
#endif

