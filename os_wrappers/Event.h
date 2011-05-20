/*
 * Event.h
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

#ifdef _WIN32
#include <windows.h>
#else
#include <semaphore.h>
#endif

/**
 * @brief Simple wrapper for OS synchronization primitive.
 *
 * This class wraps Win32 and POSIX semaphore.
 */
class Event
{
public:
    /**
     * @brief Constructor creating new event.
     */
    Event();

    /**
     * @brief Destructor that destroys the event.
     */
    virtual ~Event();

    /**
     * @brief Wait on the event for specified time.
     *
     * By default it waits for infinite time and blocks the thread until
     * the event is signaled.
     */
    void wait(unsigned long timeout = -1);

    /**
     * @brief Signal the event.
     *
     * When event is signaled the waiting thread is released.
     */
    void signal();
private:
#ifdef _WIN32
    HANDLE semaphore_;
#else
    sem_t semaphore_;
#endif
};

/* @} */
