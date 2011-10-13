/*
 * LibusbDevice_win.h
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
#ifdef _WIN32
#include <usb.h>
#else
#include <libusb.h>
#endif
#include "CommDevice.h"

// Timeout error code for usb transfer
#define USB_TIMEOUT_ERROR -116

class LibusbDevice : public CommDevice
{
public:
    LibusbDevice(struct usb_device* device);
    ~LibusbDevice();

    int read(void* buffer, size_t size);
    int write(void* buffer, size_t size);

    int getVendorId();
    struct usb_device* getPort();

    unsigned long getPhysicalAddress() {
        return 0;
    }
private:
    struct usb_device* device_;
    struct  usb_dev_handle* handle_;
    int interfaceNumber_;
    int outEndpoint_;
    int inEndpoint_;
    bool opened_;
    
    static const int READ_BUFFER_SIZE = 4096;
    char readBuffer_[READ_BUFFER_SIZE];
    char* readBufferHead_;
    size_t readBufferLength_;
};
