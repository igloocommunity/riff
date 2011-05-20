/*
 * CommException.cpp
 *
 * Copyright (C) ST-Ericsson SA 2011
 * Authors: Srimanta Panda <srimanta.panda@stericsson.com>,
 *          Ola Borgelin <ola.borgelin@stericsson.com>,
 *          Karin Hedlund <karin.hedlund@stericsson.com>,
 *          Markus Andersson <markus.m.andersson@stericsson.com> for ST-Ericsson.
 * License terms: 3-clause BSD license
 *
 */

#include "CommException.h"
#include <stdexcept>
#include <sstream>
using namespace std;

CommException::CommException(const string& message, DeviceStatus_t error):
        runtime_error(message), error_(error)
{
}

CommException::~CommException() throw()
{
}

const char* CommException::what()
{
    ostringstream message;
    message << this->runtime_error::what() << " " << error_;
    message_ = message.str();
    return message_.c_str();
}

DeviceStatus_t CommException::getError()
{
    return error_;
}
