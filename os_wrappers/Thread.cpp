/*
 * Thread.cpp
 *
 * Copyright (C) ST-Ericsson SA 2011
 * Authors: Srimanta Panda <srimanta.panda@stericsson.com>,
 *          Ola Borgelin <ola.borgelin@stericsson.com>,
 *          Karin Hedlund <karin.hedlund@stericsson.com>,
 *          Markus Andersson <markus.m.andersson@stericsson.com> for ST-Ericsson.
 * License terms: 3-clause BSD license
 *
 */

/*
 * @addtogroup OS Wrappers
 * @{
 */

#include "Thread.h"
#ifdef _WIN32
#include <process.h>
#else
#include <pthread.h>
#endif

Thread::Thread(ThreadFunction_t function, void* argument): function_(function), argument_(argument), end_(new Event())
{
#ifdef _WIN32
    _beginthread(ThreadFunction, 0, this);
#else
    pthread_attr_init(&attributes_);
    pthread_attr_setdetachstate(&attributes_, PTHREAD_CREATE_DETACHED);
    pthread_create(&id_, &attributes_, ThreadFunction, this);
#endif
}

Thread::~Thread()
{
#ifndef _WIN32
    pthread_cancel(id_);
    pthread_attr_destroy(&attributes_);
#endif
    delete end_;
}

void Thread::wait(unsigned long timeout)
{
    end_->wait(timeout);
    // signal the event again to avoid locking infinitely on following calls
    end_->signal();
}

#ifdef _WIN32
void Thread::ThreadFunction(void* arg)
#else
void* Thread::ThreadFunction(void* arg)
#endif
{
    Thread* pthis = static_cast<Thread*>(arg);
    pthis->function_(pthis->argument_);
    pthis->end_->signal();
#ifdef _WIN32
    _endthread();
#else
    return 0;
#endif
}

/* @} */
