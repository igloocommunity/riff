/*
 * LibusbDevice_win.cpp
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
#include "LibusbDevice_win.h"
#include "Debug.h"
#include "CommException.h"
#include <cstdio>
#include <iostream>

using namespace std;

#define MIN(A, B) ((A) < (B) ? (A) : (B))

LibusbDevice::LibusbDevice(struct usb_device* device): 
    device_(device),
    inEndpoint_(-1),
    outEndpoint_(-1)
{
    // get the handle for the device
    handle_ = usb_open(device_);
    if (handle_ == NULL) {
        throw CommException("failed to open usb device", COMM_DEVICE_LIBUSB_FAILED_TO_OPEN_PORT);
    }

    int configValue_ = device_->config->bConfigurationValue;

    if (usb_set_configuration(handle_, configValue_)) {
        Debug::error("Setting configuration failed!" );
        throw CommException("Setting configuration failed!", COMM_DEVICE_LIBUSB_FAILED_TO_SET_CONFIG);
    } else {
        Debug::info("Configuration has been set successfully..");
    }

    if ((this->device_->config->bNumInterfaces > 0) && (this->device_->config->interface->num_altsetting > 0)) {
        interfaceNumber_ = device_->config->interface->altsetting->bInterfaceNumber;

#if defined(LIBUSB_HAS_GET_DRIVER_NP)
#if defined(LIBUSB_HAS_DETACH_KERNEL_DRIVER_NP)
        char name[256];
        if (usb_get_driver_np(handle_, interfaceNumber_, (char *) name, sizeof(name))) {
            Debug::info("Fetching driver name failed: %s", usb_strerror());
        } else {
            Debug::info("Driver name: %s", name);
            if (usb_detach_kernel_driver_np(handle_, interfaceNumber_)) {
                Debug::error("Detaching kernel driver failed: %s", usb_strerror());
            }
            Debug::info(""Kernel driver has been detached successfully.");
        }
#endif
#endif

        // Claim the interface for the device
        if (usb_claim_interface(handle_, interfaceNumber_)) {
            opened_ = false;
            Debug::error("Claiming interface failed: %s", usb_strerror());
            throw CommException("Claiming interface failed", COMM_DEVICE_LIBUSB_FAILED_TO_CLAIM_INTERFACE);
        } else {
            Debug::info("Interface has been claimed successfuly..");
            opened_ = true;
        }

        Debug::info("Looking for endpoints..");

        //find default endpoints
        for (int i = 0; i < device_->config->interface->altsetting->bNumEndpoints; i++) {
            if ((inEndpoint_ != -1) && (outEndpoint_ != -1)) break;
            if (device_->config->interface->altsetting->endpoint[i].bEndpointAddress & 0x80) {
                if ((inEndpoint_ == -1)
                    && ((device_->config->interface->altsetting->endpoint[i].bmAttributes & 0x3f) == 0x2))
                {
                    inEndpoint_ = device_->config->interface->altsetting->endpoint[i].bEndpointAddress;
                    Debug::info("Bulk IN endpoint found and ready to use on address %02xh", inEndpoint_);
                }
            } else {
                if ((outEndpoint_ == -1)
                    && ((device_->config->interface->altsetting->endpoint[i].bmAttributes & 0x3f) == 0x2))
                {
                    outEndpoint_ = device_->config->interface->altsetting->endpoint[i].bEndpointAddress;
                    Debug::info("Bulk OUT endpoint found and ready to use on address %02xh", outEndpoint_);
                }
            }
        }

        if ((inEndpoint_ == -1) || (outEndpoint_ == -1)) {
            Debug::error("Bulk endpoints cannot be found!");
            throw CommException("Not able to set endpoints for device", COMM_DEVICE_LIBUSB_FAILED_TO_OPEN_PORT);
        }
    }
    readBufferLength_ = 0;
    readBufferHead_ = readBuffer_;
}

LibusbDevice::~LibusbDevice()
{
    usb_release_interface(handle_, interfaceNumber_);
    usb_close(handle_);
}

int LibusbDevice::read(void *buffer, size_t size)
{
    Debug::info("LibusbDevice read: called with size %d", size);
    unsigned char* dest = static_cast<unsigned char*>(buffer);

    while (true) {
        if (readBufferLength_) {
            size_t toCopy = MIN(readBufferLength_, size);
            memcpy(dest, readBufferHead_, toCopy);
            dest += toCopy;
            size -= toCopy;
            readBufferLength_ -= toCopy;

            if (readBufferLength_ != 0) {
                readBufferHead_ += toCopy;
            } else {
                readBufferHead_ = readBuffer_;
            }
        }

        if (size == 0) {
            break;
        }

        int transfered;
        transfered = usb_bulk_read(handle_, inEndpoint_, readBuffer_, READ_BUFFER_SIZE, 100000);

        if (isReadCanceled_) {
            Debug::info("LibusbDevice read: canceled...");
            Debug::hexdump("read canceled", dest, transfered);
            isReadCanceled_ = false;
            return COMM_DEVICE_CANCEL;
        }

        if (transfered < 0) {
            if( transfered == USB_TIMEOUT_ERROR) {
                Debug::info("LibusbDevice read : Usb read timeout");
                continue;
            } else {         
                Debug::info("LibusbDevice read: error %d occured", transfered);
                return transfered;
            }
        }

        readBufferLength_ = transfered;
    }

    Debug::info("LibusbDevice read: complete");
    return 0;
}

int LibusbDevice::write(void *buffer, size_t size)
{
    Debug::info("LibusbDevice write: called with size %d", size);
    char* src = static_cast<char*>(buffer);

    while (size) {
        int transfered;
        //Call with timeout to enable possible cancel
        transfered = usb_bulk_write(handle_, outEndpoint_, src, size, 100000);
        if (transfered < 0) {
            if( transfered == USB_TIMEOUT_ERROR) {
                Debug::info("LibusbDevice write : Usb write timeout");
                continue;
            } else {
                Debug::info("LibusbDevice write: error %d occured", transfered);
                return transfered;
            }
        }

        size -= transfered;
        src += transfered;
    }

    Debug::info("LibusbDevice write: complete");
    return 0;
}

int LibusbDevice::getVendorId()
{
    return device_->descriptor.idVendor;
}

struct usb_device* LibusbDevice::getPort()
{
    return device_;
}
