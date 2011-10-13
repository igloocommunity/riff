/*
 * stdafx.h
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


#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <process.h>
#include <dbt.h>

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <usb.h>
#else
#include <libusb.h>
#endif // _WIN32
