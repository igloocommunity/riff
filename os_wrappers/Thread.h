/*
 * Thread.h
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

#pragma once

#include "Event.h"
#ifndef _WIN32
#include <pthread.h>
#endif

/**
 * @brief Simple wrapper for OS thread.
 *
 * This class wraps Win32 and POSIX thread.
 */
class Thread
{
public:
    /**
     * @brief Type for thread's start function.
     */
    typedef void*(*ThreadFunction_t)(void*);

    /**
     * @brief Constructor creating new thread
     *
     * It creates new thread and starts execution of the function.
     *
     * @param[in] function - thread's start function.
     * @param[in] arg      - argument to be passed to the function.
     */
    Thread(ThreadFunction_t function, void* arg);

    /**
     * @brief Destructor.
     */
    virtual ~Thread();

    /**
     * @brief Wait for the thread to die for specified timeout.
     *
     * @param[in] timeout - Time in ms to wait. By default timeout is infinite.
     */
    void wait(unsigned long timeout = -1);
private:
#ifdef _WIN32
    static void ThreadFunction(void* arg);
#else
    pthread_t id_;
    pthread_attr_t attributes_;
    static void* ThreadFunction(void* arg);
#endif
    ThreadFunction_t function_;
    void* argument_;

    Event* end_;
};

/* @} */
