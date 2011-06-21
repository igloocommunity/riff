/*
 * SequenceFactory.h
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
 *
 * Setup execution sequence for different mode (erase/flash).
 */

#pragma once
#include "Config.h"
#include <vector>

// Command class forward declaration
class Command;

typedef std::vector<Command*> CommandSequence_t;

class SequenceFactory
{
public:
    /**
     * @brief Sequence ID.
     */
    enum SequenceId {
        BOARD_FLASH,       	/**< Board flash sequence.>  */
        BOARD_ERASE,        /**< Board erase sequence. > */
        BOARD_DUMP			/**< Board dump sequence. > */
    };

    /**
     * @brief Protocol type.
     */
    enum UseBulk {
        USE_COMMAND_PROTOCOL,       /**< Use command protocol. > */
        USE_BULK_PROTOCOL           /**< Use bulk protocol. > */
    };

    /**
     * @brief Delete buffer mode.
     */
    enum DeleteBuffer {
        DELETE_BUFFER_NO,       /**< Don't delete buffer. > */
        DELETE_BUFFER_YES       /**< Delete buffer. > */
    };

    /**
     * @brief Get new command sequence for the specified sequence ID.
     *
     * @param[in] id - ID of the sequence.
     * @return Newly created command sequence.
     */
    static CommandSequence_t getSequence(SequenceId id);

    /**
     * @brief Get new command sequence for the sequence ID specified in the profile.
     *
     * @return Newly created command sequence.
     */
    static CommandSequence_t getProfileSequence();

    /**
     *
     */
    static void setArguments(const char* configPath, const char* mode, const char* flashImg, const char* address, const char* length, const char* dumpPath);

    static const char* configPath_;
    static const char* flashmode_;
    static const char* flashImgPath_;
    static const char* dumpHostPath_;
    static int address_;
    static int length_;
private:
    SequenceFactory();
    SequenceFactory(const SequenceFactory&);
    ~SequenceFactory();

    static const SequenceId defaultSequence_;

};

/* @} */
