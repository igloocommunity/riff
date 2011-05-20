/*
 * CommDevice.h
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

#include "AsyncCommunication.h"
#include "CDAL.h"
#include <cstring>

struct CommDevice {
public:
    enum Status {
        COMM_DEVICE_SUCCESS,
        COMM_DEVICE_CANCEL,
        COMM_DEVICE_ERROR,
        COMM_DEVICE_CLOSED
    };

    CommDevice();
    virtual ~CommDevice();

    virtual int read(void* buffer, size_t size) = 0;
    int readNoWait(void* buffer, size_t size, DataCallback_t cb, void* param);

    virtual int write(void* buffer, size_t size) = 0;
    int writeNoWait(void* buffer, size_t size, DataCallback_t cb, void* param);

    int cancel();

    virtual unsigned long getPhysicalAddress() = 0;

    void* getObject() {
        return object_;
    }
    void** getObjectStorage() {
        return &object_;
    }
protected:
    bool isReadCanceled_;
private:
    void* object_;
    AsyncCommunication* async_;
};
