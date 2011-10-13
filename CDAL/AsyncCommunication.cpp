/*
 * AsyncCommunication.cpp
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
#include "AsyncCommunication.h"
#include "CommDevice.h"
#include "Debug.h"

AsyncCommunication::AsyncCommunication(CommDevice* device):
	device_(device)
{
    isReadInvoked_ = false;
    isWriteInvoked_ = false;

    readRequest_.isActive = false;
    readRequest_.isShutdown = false;
    readRequest_.callback = NULL;

    writeRequest_.isActive = false;
    writeRequest_.isShutdown = false;
    writeRequest_.callback = NULL;

    readerThread_ = new Thread(AsyncCommunication::threadReadProc, this);
    writerThread_ = new Thread(AsyncCommunication::threadWriteProc, this);
}

AsyncCommunication::~AsyncCommunication(void)
{
    readRequest_.isShutdown = true;
    readRequestEvent_.signal();
    writeRequest_.isShutdown = true;
    writeRequestEvent_.signal();
    readerThread_->wait();
    writerThread_->wait();
    delete readerThread_;
    delete writerThread_;
}

int AsyncCommunication::readNoWait(void* buf, int len, DataCallback_t fn, void* param)
{
    int res = -1;
    CSLock lock(&readCs_);

    do {
        if (readRequest_.isActive)
            break;

        readRequest_.buf = buf;
        readRequest_.length = len;
        readRequest_.isActive = true;
        readRequest_.callback = fn;
        readRequest_.param = param;
        readRequestEvent_.signal();
        res = 0;
    } while (0);

    return res;
}

int AsyncCommunication::writeNoWait(void* buf, int len, DataCallback_t fn, void* param)
{
    int res = -1;
    CSLock lock(&writeCs_);

    do {
        if (writeRequest_.isActive)
            break;

        writeRequest_.buf = buf;
        writeRequest_.length = len;
        writeRequest_.isActive = true;
        writeRequest_.callback = fn;
        writeRequest_.param = param;
        writeRequestEvent_.signal();
        res = 0;
    } while (0);

    return res;
}

void* AsyncCommunication::threadReadProc(void* param)
{
    AsyncCommunication* asyncComm = (AsyncCommunication*)param;

    while (1) {
        asyncComm->readRequestEvent_.wait();

        if (asyncComm->readRequest_.isShutdown) {
            Debug::info("read thread: shutdown requested");
            return 0;
        }

        asyncComm->readCs_.enter();
        unsigned char* destBuf = static_cast<unsigned char*>(asyncComm->readRequest_.buf);
        int length = asyncComm->readRequest_.length;
        void* param = asyncComm->readRequest_.param;
        DataCallback_t callback = asyncComm->readRequest_.callback;
        asyncComm->readCs_.leave();

        int readResult = asyncComm->device_->read(destBuf, length);

        asyncComm->readCs_.enter();
        asyncComm->readRequest_.isActive = false;
        asyncComm->readCs_.leave();

        if (readResult != CommDevice::COMM_DEVICE_SUCCESS) {
            if (readResult == CommDevice::COMM_DEVICE_CANCEL) {
                asyncComm->readCanceledEvent_.signal();
            }

            // if error occurs ignore the current request and wait for next one
            // LCM should send the same request again after timeout
            continue;
        }

        if (callback) {
            Debug::hexdump("read", destBuf, length);
            callback(destBuf, length, param);
        }
    }

    return 0;
}

void* AsyncCommunication::threadWriteProc(void *param)
{
    AsyncCommunication* asyncComm = (AsyncCommunication*)param;

    while (1) {
        asyncComm->writeRequestEvent_.wait();

        if (asyncComm->writeRequest_.isShutdown) {
            Debug::info("write thread: shutdown requested");
            return 0;
        }

        asyncComm->writeCs_.enter();
        unsigned char* srcBuf = static_cast<unsigned char*>(asyncComm->writeRequest_.buf);
        int len = asyncComm->writeRequest_.length;
        void* localParam = asyncComm->writeRequest_.param;
        DataCallback_t callback = asyncComm->writeRequest_.callback;
        asyncComm->writeCs_.leave();

        int writeResult = asyncComm->device_->write(srcBuf, len);

        asyncComm->writeCs_.enter();
        asyncComm->writeRequest_.isActive = false;
        asyncComm->writeCs_.leave();

        if (writeResult != CommDevice::COMM_DEVICE_SUCCESS) {
            // if error occurs ignore the current request and wait for next one
            // LCM should send the same request again after timeout
            continue;
        }

        if (callback) {
            Debug::hexdump("write", srcBuf, len);
            callback(srcBuf, len, localParam);
        }
    }

    return 0;
}

void AsyncCommunication::waitReadCanceled()
{
    readCanceledEvent_.wait();
    Debug::info("AsyncCommunication waitReadCanceled finished");
}
