/*
 * InitializeDut.h
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
 * @addtogroup Commands
 * @{
 */

#pragma once
#include "Command.h"
#include "Config.h"
#include "DUT.h"
#include <string>
#include <cstdio>
using namespace std;

struct tocSection {
    int start;
    int size;
    int flags;
    int align;
    int loadAddress;
    char filename[12];
};

/**
 * @brief Class implementing command for DUT initialization.
 */
class InitializeDut : public Command
{
public:
    /**
     * @brief Default constructor taking a config file path as parameter.
     */
    InitializeDut(const char* configPath);

    /**
     * @brief Method that executes the DUT initialization.
     *
     * This method initializes new LCD context and sets the DUT in service mode.
     *
     * @param[in] dut - Specific DUT instance on which to execute the command.
     * @return int    - Command execution error (0 on success, non-0 otherwise).
     */
    int run(DUT* dut);

    const char * get_command_name();

private:
    const char* configPath_;
    const char* isswPath_;
    const char* xloaderPath_;
    const char* meminitPath_;
    const char* pwrmgmtPath_;
    const char* normalPath_;

    char tocArea[512];
    tocSection issw_, xload_, meminit_, normal_, pwrMgt_;

    Logger logger_;

    int commWrite2kChunks(Device_t device, void* buffer, size_t size);
    int sendTocIssw(Device_t device);
    int createTocEntry(tocSection* issw, tocSection* xload, tocSection* meminit, tocSection* normal, tocSection* pwrMgt);
    int createTocFile(tocSection* toc, const char* filePath, const char* tocName, int* offset);
    int getFileSize(const char* filePath);
    int readIsswFile(char* buf);
    int readSecondIsswFile(char* buf);
    int readPwrMgtFile(char* buf);
    int readMeminitFile(char* buf);
    int readNormal(char* buf);
    int initializeHardware(Device_t device);
    void displayLCDError(const char* message, uint32 errorno);

};

/* @} */
