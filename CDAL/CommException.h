/*
 * CommException.h
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
#include <stdexcept>
#include <string>

class CommException : public std::runtime_error
{
public:
    CommException(const std::string& message, DeviceStatus_t error);
    virtual ~CommException() throw();
    virtual const char* what();
    DeviceStatus_t getError();
private:
    DeviceStatus_t error_;
    std::string message_;
};
