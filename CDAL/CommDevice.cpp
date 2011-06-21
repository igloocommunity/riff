/*
 * CommDevice.cpp
 *
 * Copyright (C) ST-Ericsson SA 2011
 * Authors: Srimanta Panda <srimanta.panda@stericsson.com>,
 *          Ola Borgelin <ola.borgelin@stericsson.com>,
 *          Karin Hedlund <karin.hedlund@stericsson.com>,
 *          Markus Andersson <markus.m.andersson@stericsson.com> for ST-Ericsson.
 * License terms: 3-clause BSD license
 *
 */

#include "CommDevice.h"
#include "AsyncCommunication.h"

CommDevice::CommDevice(): isReadCanceled_(false)
{
    async_ = new AsyncCommunication(this);
}

CommDevice::~CommDevice()
{
    delete async_;
}

int CommDevice::readNoWait(void *buffer, size_t size, DataCallback_t cb, void *param)
{
    return async_->readNoWait(buffer, size, cb, param);
}

int CommDevice::writeNoWait(void *buffer, size_t size, DataCallback_t cb, void *param)
{
    return async_->writeNoWait(buffer, size, cb, param);
}

int CommDevice::cancel()
{
    if (async_->isReadActive()) {
        isReadCanceled_ = true;
        async_->waitReadCanceled();
    }

    return 0;
}
