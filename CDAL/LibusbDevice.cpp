/*
 * LibusbDevice.cpp
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
#include "LibusbDevice.h"
#include "Debug.h"
#include "CommException.h"
#include <libusb.h>

using namespace std;

#define MIN(A, B) ((A) < (B) ? (A) : (B))

#define CHUNKSIZE 16384 //16 kB

LibusbDevice::LibusbDevice(libusb_device* device): device_(device)
{
    int status = libusb_open(device_, &handle_);

    if (status != LIBUSB_SUCCESS) {
        throw CommException("failed to open usb device", COMM_DEVICE_LIBUSB_FAILED_TO_OPEN_PORT);
    }

    status = libusb_claim_interface(handle_, 0);

    if (status != LIBUSB_SUCCESS) {
        libusb_close(handle_);
        throw CommException("failed to claim device interface", COMM_DEVICE_LIBUSB_FAILED_TO_CLAIM_INTERFACE);
    }

    libusb_config_descriptor* config;
    libusb_get_active_config_descriptor(device, &config);
    Debug::info("LibusbDevice: found %d endpoints", config->interface->altsetting->bNumEndpoints);

    for (int i = 0; i != config->interface->altsetting->bNumEndpoints; ++i) {
        if (config->interface->altsetting->endpoint[i].bEndpointAddress & LIBUSB_ENDPOINT_IN) {
            inEndpoint_ = config->interface->altsetting->endpoint[i].bEndpointAddress;
            Debug::info("LibusbDevice: in endpoint set to 0x%02x", inEndpoint_);
        } else {
            outEndpoint_ = config->interface->altsetting->endpoint[i].bEndpointAddress;
            Debug::info("LibusbDevice: out endpoint set to 0x%02x", outEndpoint_);
        }
    }

    libusb_free_config_descriptor(config);

    libusb_get_device_descriptor(device, &descriptor_);

    readBufferLength_ = 0;
    readBufferHead_ = readBuffer_;
}

LibusbDevice::~LibusbDevice()
{
    libusb_release_interface(handle_, 0);
    libusb_close(handle_);
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
        int error = libusb_bulk_transfer(handle_, inEndpoint_, readBuffer_, READ_BUFFER_SIZE, &transfered, 100000);

        if (isReadCanceled_) {
            Debug::info("LibusbDevice read: canceled...");
            Debug::hexdump("read canceled", dest, transfered);
            isReadCanceled_ = false;
            return COMM_DEVICE_CANCEL;
        }

        if (error) {
            if (error == LIBUSB_ERROR_TIMEOUT) {
                continue;
            } else {
                Debug::info("LibusbDevice read: error %d occured", error);
                return error;
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
    unsigned char* src = static_cast<unsigned char*>(buffer);

    while (size) {
        int transfered;
        int chunkSize = size > CHUNKSIZE ? CHUNKSIZE : size;

        //Call with timeout to enable possible cancel
        int error = libusb_bulk_transfer(handle_, outEndpoint_, src, chunkSize, &transfered, 0);

        if (error) {
            if (error == LIBUSB_ERROR_TIMEOUT) {
                continue;
            } else {
                return error;
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
    return descriptor_.idVendor;
}

libusb_device* LibusbDevice::getPort()
{
    return device_;
}
