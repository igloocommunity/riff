/*
 * SequenceFactory.cpp
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
 * @addtogroup SequenceFactory
 * @{
 */

#include "Config.h"
#include "SequenceFactory.h"
#include "Command.h"
#include "InitializeDut.h"
#include "ProcessRawImage.h"
#include "Shutdown.h"
#include "EraseArea.h"
#include "DumpArea.h"
#include "constants.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

const SequenceFactory::SequenceId SequenceFactory::defaultSequence_ = SequenceFactory::BOARD_FLASH;

const char* SequenceFactory::configPath_;
const char* SequenceFactory::flashmode_;
const char* SequenceFactory::flashImgPath_;
const char* SequenceFactory::dumpHostPath_;
int SequenceFactory::address_ = 0;
int SequenceFactory::length_ = 0;

void SequenceFactory::setArguments(const char* configPath, const char* mode, const char* flashImg, const char* address, const char* length, const char* dumpPath)
{
    SequenceFactory::configPath_ = configPath;
    SequenceFactory::flashmode_ = mode;
    SequenceFactory::flashImgPath_ = flashImg;
    SequenceFactory::address_ = strtol(address, NULL, 16);
    SequenceFactory::length_ = strtol(length, NULL, 16);
    SequenceFactory::dumpHostPath_ = dumpPath;
}

CommandSequence_t SequenceFactory::getSequence(SequenceId id)
{
    CommandSequence_t sequence;

    // Initialize DUT
    sequence.push_back(new InitializeDut(configPath_));


    switch (id) {
    case BOARD_FLASH: {
        // Process raw software image
        sequence.push_back(new flash::ProcessRawImage(flashImgPath_, address_, USE_BULK_PROTOCOL, DELETE_BUFFER_YES));
    }
    break;
    case BOARD_ERASE: {
        // erase complete flash
        sequence.push_back(new flash::EraseArea(flash::FLASH_DEVICE_TYPE, 0, -1));
    }
    break;
    case BOARD_DUMP: {
		// dump flash area
		sequence.push_back(new flash::DumpArea(flash::FLASH_DEVICE_TYPE, address_, length_, dumpHostPath_, 1, USE_BULK_PROTOCOL));
	}
	break;

    default:
        cout << "No action to be performed" << endl;

    }

    // shut down
    sequence.push_back(new System::Shutdown());

    return sequence;
}

CommandSequence_t SequenceFactory::getProfileSequence()
{
    if (SequenceFactory::flashmode_ == string("flash")) {
        return getSequence(BOARD_FLASH);
    }

    if (SequenceFactory::flashmode_ == string("erase")) {
        return getSequence(BOARD_ERASE);
    }

    if (SequenceFactory::flashmode_ == string("dump")) {
    	return getSequence(BOARD_DUMP);
    } else {
        return getSequence(defaultSequence_);
    }
}

/* @} */
