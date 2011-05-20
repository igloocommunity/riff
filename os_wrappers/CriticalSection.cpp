/*
 * CriticalSection.cpp
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

#include "CriticalSection.h"

CriticalSection::CriticalSection()
{
#ifdef _WIN32
    InitializeCriticalSection(&mutex_);
#else
    pthread_mutex_init(&mutex_, NULL);
#endif
}

CriticalSection::~CriticalSection()
{
#ifdef _WIN32
    DeleteCriticalSection(&mutex_);
#else
    pthread_mutex_destroy(&mutex_);
#endif
}

void CriticalSection::enter()
{
#ifdef _WIN32
    EnterCriticalSection(&mutex_);
#else
    pthread_mutex_lock(&mutex_);
#endif
}

void CriticalSection::leave()
{
#ifdef _WIN32
    LeaveCriticalSection(&mutex_);
#else
    pthread_mutex_unlock(&mutex_);
#endif
}

CSLock::CSLock(CriticalSection* section): section_(section)
{
    section_->enter();
}

CSLock::~CSLock()
{
    section_->leave();
}

/* @} */
