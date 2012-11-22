/*
 * main.cpp
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
#include "main.h"
#include "CDAL.h"
#include "DutManager.h"
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>

using namespace std;

volatile bool isDone = false;
const string info = \
                    "\n \
----------------------- riff - Raw Image File Flasher -------------------------\n \
Version: 0.5.2\n \
"
                    "Flash a device. Try `riff --help' for more information. \n \
"
                    "-------------------------------------------------------------------------------";

const string cmdHelpString = " \n \
Usage: riff [OPTION]...[FILE]...\n\n \
Available command line arguments are:\n\n \
  -h, --help\t			Display this help message.\n \
  -m, --mode ACTION\t		Select the mode.\n \
					ACTION is `flash`, `erase` or `dump`\n \
  -c, --config PATH\t		Give path to configuration file.\n \
  -f, --flashimage IMAGEPATH\t	Give path to flashimage.\n \
  -a, --address FLASHADDR\t	Give a start address in hex.\n \
  -l, --length HEXLENGTH		Length of the dump [Byte] in hex.\n \
  -d, --dumppath DUMPPATH		File path on PC where to store dump.\n \
  -v, --verbose\t		Shows more detailed information.\n \
      --version\t		Shows version information.\n \
";


int main(int argc, char* argv[])
{
    signal(SIGINT, interrupt);

    cout << info << endl;

    handleCmdLineInput(argc, argv);

    logger_ = new Logger("Application");
    logger_->log(Logger::INFO, "Using config file %s", configFile);

    //Parse the config file.
    config_ = new Config(configFile);

    // if flashimage path has not been set from commandline
    //then use the path in the config file if it is set there.
    if (!strlen(flashimage)) {
        if (config_->valueExists("FLASH_IMG_PATH")) {
            strcpy(flashimage, config_->getValue("FLASH_IMG_PATH"));
        }
    }

    // Set the LCM path from ConfigFile, init the USB driver and set the callback function
    SetLCMLibPath(config_->getValue("LCMLIBPATH"));
    usb_init_driver(config_->getValue("VENDORID"), config_->getValue("PRODUCTID"));
    usb_set_listen_callback(UsbDeviceEventCallback);


    logger_->log(Logger::PROGRESS, "Listening on USB for device connection...");

    while (!isDone) {
#ifdef _WIN32
        Sleep(1000);
#else
        sleep(1);
#endif
    }

    usb_deinit_driver();

    _exit(exitstatus);
}

void interrupt(int param)
{
    logger_->log(Logger::PROGRESS, "Program interrupted...");
    exitstatus = 1;
    isDone = true;
}

void UsbDeviceEventCallback(DeviceStatus_t status, DeviceEvent_t event, Device_t device)
{
    if (COMM_DEVICE_SUCCESS == status) {
        const DUT* dut;

        switch (event) {
        case LIBUSB_DEVICE_CONNECTED:

            logger_->log(Logger::INFO, "Device detected on USB@%u", comm_get_physical_address(device));
            dut = DutManager::createDut(device);

            if (0 != dut) {
                logger_->log(Logger::PROGRESS, "Connected %s", dut->getId());
            }

            break;
        case LIBUSB_DEVICE_DISCONNECTED:
            logger_->log(Logger::INFO, "Disconnect detected on USB@%u", comm_get_physical_address(device));
            dut = DutManager::getDut(device);

            if (0 != dut) {
                exitstatus = dut->getErrorCode();
                logger_->log(Logger::PROGRESS, "Disconnected %s", dut->getId());
                DutManager::destroyDut(device);
            }

            isDone = true;
            break;
        default:
            logger_->log(Logger::ERR, "Unknown USB event %d", event);
            break;
        }
    } else if (COMM_DEVICE_LIBUSB_FAILED_TO_OPEN_PORT == status) {
        logger_->log(Logger::ERR, "Cannot open USB device. Are you root?", status);
        isDone = true;
        exitstatus = 1;
    } else {
        logger_->log(Logger::ERR, "USB device error %d", status);
    }
}
void handleCmdLineInput(int argc, char* argv[])
{
    string input;
    bool configPathSet = false;

    for (int i = 1; i != argc; ++i) {
        input = argv[i];

        if ("-h" == input || "--help" == input) {
            cout << cmdHelpString << endl;
            _exit(0);
        }

        else if ("-v" == input || "--verbose" == input) {
            Logger::verbose_ = true;
        }

        else if ("--version" == input) {
            _exit(0);
        }

        else if ("-m" == input || "--mode" == input) {
            i = checkCmdLineArgument(argc, i,  argv, mode);
        }

        else if ("-c" == input || "--config" == input) {
            configPathSet = true;
            i = checkCmdLineArgument(argc, i,  argv, configFile);

        }

        else if ("-f" == input || "--flashimage" == input) {
        	i = checkCmdLineArgument(argc, i,  argv, flashimage);
        }

        else if ("-a" == input || "--address" == input) {
        	i = checkCmdLineArgument(argc, i,  argv, address);
        }

        else if ("-l" == input || "--length" == input) {
        	i = checkCmdLineArgument(argc, i,  argv, length);
        }

        else if ("-d" == input || "--dumppath" == input) {
        	i = checkCmdLineArgument(argc, i,  argv, dumpPath);
        } else {
            cout << "Unknown option: " << input << endl;
        }
    }

    if (!configPathSet) {
        FILE* userConfig;
#ifdef _WIN32
        char* home = getenv("USERPROFILE");
#else
        char* home = getenv("HOME");
#endif
        char homeConfigPath[50];
        strcpy(homeConfigPath, home);
        strcat(homeConfigPath, "/.riff/config");


        userConfig = fopen(homeConfigPath, "rb");

        if (userConfig != NULL) {
            strcpy(configFile, homeConfigPath);
            fclose(userConfig);
        }
        else {
             // It will check the default config in /usr/share folder otherwise
             // or /<current directory>/riff/config
             userConfig = fopen(configFile, "r");
             if(userConfig == NULL) {
                 cout << cmdHelpString << endl;
                 _exit(0);
             }
        }


    }
    if (*dumpPath == '\0') {
       //Sets default dump path if not provided
#ifdef _WIN32
       char* home = ".";
#else
       char* home = getenv("HOME");
#endif
       strcpy(dumpPath, home);
       strcat(dumpPath, "/flashdump.bin");
    }
    SequenceFactory::setArguments(configFile, mode, flashimage, address, length, dumpPath);
}

int checkCmdLineArgument(int argc, int counter, char* argv[], char* var)
{
    counter++;

    if (counter < argc)

    {
        strcpy(var, argv[counter]);
    } else {
        cout << "Please give a parameter to the option" << endl;
        counter--;
    }

    return counter;
}

/* @} */
