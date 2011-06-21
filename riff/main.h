/*
 * main.h
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
 * @addtogroup main
 * @{
 */
#pragma once

#include "CDAL.h"
#include "Config.h"
#include <string>
#include "Logger.h"
#include <signal.h>
#define PATH_LENGTH 256
#define MODE_STR_LENGTH 6

enum ConnectionType {
    USB_CONNECTION, /**< USB connection. */
};


Logger* logger_;
Config* config_;
char configFile[PATH_LENGTH] = "/usr/share/riff/config";
char flashimage[PATH_LENGTH] = "\0";
char address[PATH_LENGTH] = "0";
char length[PATH_LENGTH] = "0";
char dumpPath[PATH_LENGTH] = "\0";
char mode[MODE_STR_LENGTH];
int exitstatus = 0;

/**
 * @brief USB device event callback function.
 *
 * This function is called when USB device is connected or disconnected.
 *
 * @param[in] status - Status of the device.
 * @param[in] event  - Event that was triggered.
 * @param[in] device - The device that triggered the event.
 */
void UsbDeviceEventCallback(DeviceStatus_t status, DeviceEvent_t event, Device_t device);

/**
 * @brief Handling user commands from commandline
 *
 * This function is called to handle the commands typed by the user from commandline.
 *
 * @param[in] argc - number of inparameters
 * @param[in] argv[] - inparameters given by user
 */
void handleCmdLineInput(int argc, char* argv[]);

/**
 * @brief Checking commandline arguments to the options
 *
 */
int checkCmdLineArgument(int argc, int counter, char* argv[], char* var);

/**
 * @brief Handles user interupts (CTRL+C)
 *
 */
void interrupt(int param);

/* @} */
