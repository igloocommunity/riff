/*
 * LibusbDevice.h
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
#include <libusb.h>
#include "CommDevice.h"

class LibusbDevice : public CommDevice
{
public:
    LibusbDevice(libusb_device* device);
    ~LibusbDevice();

    int read(void* buffer, size_t size);
    int write(void* buffer, size_t size);

    int getVendorId();
    libusb_device* getPort();

    unsigned long getPhysicalAddress() {
        return 0;
    }
private:
    libusb_device* device_;
    libusb_device_handle* handle_;
    libusb_device_descriptor descriptor_;
    int outEndpoint_;
    int inEndpoint_;
    static const int READ_BUFFER_SIZE = 4096;
    unsigned char readBuffer_[READ_BUFFER_SIZE];
    unsigned char* readBufferHead_;
    size_t readBufferLength_;
};
