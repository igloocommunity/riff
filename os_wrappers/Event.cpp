/*
 * Event.cpp
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

#include "Event.h"

Event::Event()
{
#ifdef _WIN32
    semaphore_ = CreateSemaphore(NULL, 0, 256, NULL);
#else
    sem_init(&semaphore_, 0, 0);
#endif
}

Event::~Event()
{
#ifdef _WIN32
    CloseHandle(semaphore_);
#else
    sem_destroy(&semaphore_);
#endif
}

void Event::signal()
{
#ifdef _WIN32
    ReleaseSemaphore(semaphore_, 1, NULL);
#else
    sem_post(&semaphore_);
#endif
}

#ifdef _WIN32
void Event::wait(unsigned long timeout)
{
    WaitForSingleObject(semaphore_, timeout);
}
#else
void Event::wait(unsigned long timeout __attribute__((unused)))
{
    sem_wait(&semaphore_);
}
#endif

/* @} */
