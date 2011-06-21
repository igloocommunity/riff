/*
 * CriticalSection.h
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

#ifndef _WIN32
#include <pthread.h>
#endif

/**
 * @brief Simple wrapper for OS synchronization primitive.
 *
 * This class wraps Win32 critical section and POSIX threads mutex.
 */
class CriticalSection
{
public:
    /**
     * @brief Constructor creating new critical section.
     */
    CriticalSection();

    /**
     * @brief Destructor that destroys the critical section.
     */
    virtual ~CriticalSection();

    /**
     * @brief This method enters/locks the critical section.
     */
    void enter();

    /**
     * @brief This method leaves/unlocks the critical section.
     */
    void leave();
private:
#ifdef _WIN32
    CRITICAL_SECTION mutex_;
#else
    pthread_mutex_t mutex_;
#endif
};

/**
 * @brief RAII class for critical section locking/unlocking.
 */
class CSLock
{
public:
    /**
     * @brief Constructor that enters/locks the passed critical section.
     *
     * @param[in] section - critical section to lock.
     */
    CSLock(CriticalSection* section);

    /**
     * @brief Destructor that leaves/unlocks the associated critical section.
     */
    virtual ~CSLock();
private:
    CriticalSection* section_;
    CSLock(const CSLock&);
    CSLock& operator=(const CSLock&);
};

/* @} */
