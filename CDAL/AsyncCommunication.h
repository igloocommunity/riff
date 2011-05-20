/*
 * AsyncCommunication.h
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
#include "Event.h"
#include "Thread.h"
#include "CriticalSection.h"

struct commRequest {
    void* buf;
    int length;
    DataCallback_t callback;
    void* param;
    bool isShutdown;
    bool isActive;
};

class AsyncCommunication
{
public:
    AsyncCommunication(CommDevice* device);
    ~AsyncCommunication();

    int readNoWait(void* buf, int len, DataCallback_t fn, void* param);
    int writeNoWait(void* buf, int len, DataCallback_t fn, void* param);

    bool isReadActive() {
        return readRequest_.isActive;
    }
    void waitReadCanceled();
private:
    CommDevice* device_;
    bool isReadInvoked_;
    bool isWriteInvoked_;

    commRequest readRequest_;
    CriticalSection readCs_;

    commRequest writeRequest_;
    CriticalSection writeCs_;

    Thread* readerThread_;
    static void* threadReadProc(void*);
    Event readRequestEvent_;
    Event readCanceledEvent_;

    Thread* writerThread_;
    static void* threadWriteProc(void*);
    Event writeRequestEvent_;
};
