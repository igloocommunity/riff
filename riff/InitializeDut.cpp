/*
 * InitializeDut.cpp
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

#include "InitializeDut.h"
#include "Logger.h"
#include "DUT.h"
#include "Utilities.h"
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sys/stat.h>

using namespace std;


int InitializeDut::readIsswFile(char* buf)
{
    int res = 0;
    FILE* isswImage;

    isswImage = fopen(isswPath_, "rb");

    if (isswImage == NULL) {
        logger_.log(Logger::ERR, "InitializeDut.cpp (%d) %s() error while opening file %s", __LINE__, __FUNCTION__, isswPath_);
        return -1;
    }

    res = fread(buf, 1, issw_.size, isswImage);

    fclose(isswImage);
    return 0;
}

int InitializeDut::readSecondIsswFile(char* buf)
{
    int res = 0;
    FILE* xloaderImage;

    xloaderImage = fopen(xloaderPath_, "rb");

    if (xloaderImage == NULL) {
        logger_.log(Logger::ERR, "InitializeDut.cpp (%d) %s() error while opening file %s", __LINE__, __FUNCTION__, xloaderPath_);
        return -1;
    }

    res = fread(buf, 1, xload_.size, xloaderImage);
    fclose(xloaderImage);
    return 0;
}

int InitializeDut::readPwrMgtFile(char* buf)
{
    FILE* pwrmgmtImage;

    pwrmgmtImage = fopen(pwrmgmtPath_, "rb");

    if (pwrmgmtImage == NULL) {
        logger_.log(Logger::ERR, "InitializeDut.cpp (%d) %s() error while opening file %s", __LINE__, __FUNCTION__, pwrmgmtPath_);
        return -1;
    }

    fread(buf, 1, pwrMgt_.size, pwrmgmtImage);
    fclose(pwrmgmtImage);
    return 0;
}

int InitializeDut::readMeminitFile(char* buf)
{
    FILE* meminitImage;

    meminitImage = fopen(meminitPath_, "rb");

    if (meminitImage == NULL) {
        logger_.log(Logger::ERR, "InitializeDut.cpp (%d) %s() error while opening file %s", __LINE__, __FUNCTION__, meminitPath_);
        return -1;
    }

    fread(buf, 1, meminit_.size, meminitImage);
    fclose(meminitImage);
    return 0;
}

int InitializeDut::readNormal(char* buf)
{
    FILE* normalImage;

    normalImage = fopen(normalPath_, "rb");

    if (normalImage == NULL) {
        logger_.log(Logger::ERR, "InitializeDut.cpp (%d) %s() error while opening file %s", __LINE__, __FUNCTION__, normalPath_);
        return -1;
    }

    fread(buf, 1, normal_.size, normalImage);
    fclose(normalImage);
    return 0;
}

int InitializeDut::commWrite2kChunks(Device_t device, void* buffer, size_t size)
{
    char* buf = static_cast<char*>(buffer);

    while (size) {
        size_t toSend = size > 2048 ? 2048 : size;

        if (comm_write(device, buf, toSend) < 0)
            return -1;

        size -= toSend;
        buf += toSend;
    }

    return 0;
}

int InitializeDut::sendTocIssw(Device_t device)
{
    //Send TOC+ISSW to the board
    int index = 0;
    tocSection* tocSections = (tocSection*)tocArea;
    unsigned int bootIndication;
    char* sendBuffer = NULL;
    int sendBufferSize = 0;
    char* tempNORMAL = (char*) "NORMAL";

    memset(tocSections, 0xff, 512);

    //Build TOC Sections List
    tocSections[index++] = issw_;

    // X-Loader section
    tocSections[index++] = xload_;
    tocSections[index++] = meminit_;

    if (strcmp(pwrMgt_.filename, tempNORMAL) == 0) {
        tocSections[index++] = normal_;
    } else {
        tocSections[index++] = pwrMgt_;
        tocSections[index++] = normal_;
    }

    sendBufferSize = 512 + issw_.size;
    sendBuffer = (char*)malloc(sendBufferSize);

    bootIndication = 0xf0030002;

    if (comm_write(device, (char*)&bootIndication, 4) < 0)
        return -1;

    if (comm_write(device, (char*)&sendBufferSize, 4) < 0)
        return -1;

    memcpy(sendBuffer, tocSections, 512);
    readIsswFile(sendBuffer + 512);

    commWrite2kChunks(device, sendBuffer, sendBufferSize);

    return 0;
}

int InitializeDut::createTocEntry(tocSection* issw, tocSection* xload, tocSection* meminit, tocSection* normal, tocSection* pwrMgt)
{
    int offset = 512;

    createTocFile(issw, isswPath_, "ISSW", &offset); //ISSW TOC
    createTocFile(xload, xloaderPath_, "X-LOADER", &offset); //X-LOADER TOC
    createTocFile(meminit, meminitPath_, "MEM_INIT", &offset); //MEMINIT TOC

    if (strlen(pwrmgmtPath_) != 0) {
        createTocFile(pwrMgt, pwrmgmtPath_, "PWR_MGT", &offset); //PWR_MGT TOC
        createTocFile(normal, normalPath_, "NORMAL", &offset); //NORMAL TOC
    } else {
        createTocFile(pwrMgt, normalPath_, "NORMAL", &offset); //NORMAL TOC
        *normal = *pwrMgt;
    }

    return 0;
}

int InitializeDut::createTocFile(tocSection* toc, const char* filePath, const char* tocName, int* offset)
{
    size_t size = 0;

    if ((filePath == NULL) || (strlen(filePath) == 0))
        return -1;

    size = getFileSize(filePath);

    // Create the TOC entry
    toc->start = *offset;
    toc->size = size;
    toc->flags = 0;
    toc->align = 0;
    toc->loadAddress = 0;
    strcpy(toc->filename, tocName);

    *offset += size;

    return 0;
}

int InitializeDut::getFileSize(const char* filePath)
{

    struct stat filestatus;
    stat(filePath, &filestatus);

    return filestatus.st_size;

}

int InitializeDut::initializeHardware(Device_t device)
{

    char asicIdUsb[65];
    unsigned char secureMode;

    unsigned int byteSynchro = 0x53548815;

    Config config(configPath_);

    isswPath_ = config.getValue("ISSWPATH");
    xloaderPath_ = config.getValue("XLOADERPATH");
    meminitPath_ = config.getValue("MEMINITPATH");
    pwrmgmtPath_ = config.getValue("PWRMGMTPATH");
    normalPath_ = config.getValue("NORMALPATH");

    if (comm_write(device, (char*)&byteSynchro, 4) < 0)
        return -1;

    if (comm_read(device, asicIdUsb, 65) < 0)
        return -1;

    secureMode = asicIdUsb[11];

    if (createTocEntry(&issw_, &xload_, &meminit_, &normal_, &pwrMgt_))
        return -1;

    if (sendTocIssw(device))
        return -1;

    while (1) {
        unsigned int reqId;

        if (comm_read(device, (char*)&reqId, 4) < 0)
            return -1;

        char *sendBuffer = NULL;

        switch (reqId) {
        case 0xA0400000:
            /* 2nd Loader ISSW Requested XLoader */
            logger_.log(Logger::INFO, "X-LOADER token");

            if (comm_write(device, (char*)&xload_.size, 4) < 0)
                return -1;

            sendBuffer = (char*)malloc(xload_.size);

            if (readSecondIsswFile(sendBuffer) < 0)
            {
            	free(sendBuffer);
            	return -1;
            }



            commWrite2kChunks(device, sendBuffer, xload_.size);
            logger_.log(Logger::INFO, "X-LOADER sent");
            free(sendBuffer);

            break;

        case 0xA0300002:
            /*PWR_MGT token*/
            logger_.log(Logger::INFO, "PWR_MGT token");

            if (comm_write(device, (char*)&pwrMgt_.size, 4) < 0)
                return -1;

            sendBuffer = (char*)malloc(pwrMgt_.size);

            if (readPwrMgtFile(sendBuffer) < 0)
            {
            	free(sendBuffer);
            	return -1;
            }

            commWrite2kChunks(device, sendBuffer, pwrMgt_.size);
            logger_.log(Logger::INFO, "PWR_MGT sent");
            free(sendBuffer);

            break;

        case 0xA0300000:
            /* Mem Init token */
            logger_.log(Logger::INFO, "MEM_INIT token");

            if (comm_write(device, (char*)&meminit_.size, 4) < 0)
                return -1;

            sendBuffer = (char*)malloc(meminit_.size);

            if (readMeminitFile(sendBuffer) < 0)
            {
            	free(sendBuffer);
            	return -1;
            }

            commWrite2kChunks(device, sendBuffer, meminit_.size);
            logger_.log(Logger::INFO, "MEM_INIT sent");
            free(sendBuffer);

            break;

        case 0xA0300001:
            /* Application token */
            logger_.log(Logger::INFO, "NORMAL token");

            if (comm_write(device, (char*)&normal_.size, 4) < 0)
                return -1;

            sendBuffer = (char*)malloc(normal_.size);

            if (readNormal(sendBuffer) < 0)
            {
            	free(sendBuffer);
            	return -1;
            }

            commWrite2kChunks(device, sendBuffer, normal_.size);
            logger_.log(Logger::INFO, "NORMAL sent");
            free(sendBuffer);

            return 0;

            break;

        default:
            logger_.log(Logger::INFO, "UNKNOWN token");
        }
    }
}
InitializeDut::InitializeDut(const char* configPath):
		logger_("InitializeDut")
{
    configPath_ = configPath;
}

int InitializeDut::run(DUT* dut)
{
    int error = 0;
    Device_t commDevice = dut->getCommDevice();
    LCDContext lcdContext = 0;

    char version[100] = {0};
    int versionSize = 100;
    char protocol[100] = {0};
    int protocolSize = 100;

    logger_.log(Logger::INFO, "Initializing device...");

    error = initializeHardware(commDevice);
    if (0 != error)
    {
        logger_.log(Logger::ERR,"ERROR: Error while initializing device %d", error);
        return error;
    }

    logger_.log(Logger::INFO, "Hardware Initialization finished");

    logger_.log(Logger::INFO, "Starting initialization of LCD...");
    void** deviceObjectStorage = comm_get_object_storage(commDevice);
    error = CreateContext(&lcdContext, dut->getId());
    if (0 != error)
    {
        displayLCDError("Error while creating LCD context", error);
        return error;
    }

    dut->setLCDContext(lcdContext);
    error = ConfigureCommunicationDevice(lcdContext, (void*)comm_read_nowait, (void*)comm_write_nowait, (void*)comm_cancel);
    if (0 != error)
    {
        displayLCDError("Error while configuring communication device", error);
        return error;
    }


    error = SwitchProtocolFamily(lcdContext, R15_PROTOCOL_FAMILY);
    if (0 != error)
    {
        displayLCDError("Error while setting protocol family", error);
        return error;
    }

    error = StartContext(lcdContext, deviceObjectStorage);
    if (0 != error)
    {
        displayLCDError("Error while starting LCD context", error);
        return error;
    }

    logger_.log(Logger::INFO, "LCD initialization finished successfully");

    error = SetProgressCallback(lcdContext, (void*)comm_progress);
    if (0 != error)
    {
        displayLCDError("Error while setting Progress listener", error);
        return error;
    }

    logger_.log(Logger::INFO, "Progress listener added successfully");

    error = System_LoaderStartupStatus(dut->getLCDContext(), version, &versionSize, protocol, &protocolSize);
    if (0 != error)
    {
        displayLCDError("Failed to receive loader startup status", error);
        return error;
    }

    logger_.log(Logger::INFO, "Loader successfully started");
    logger_.log(Logger::INFO, "  Version: %s", version);
    logger_.log(Logger::INFO, "  Protocol: %s", protocol);

    Utilities::sleep(100);

    return error;
}

const char * InitializeDut::get_command_name()
{
    return (char *)"INITIALIZE_DUT";
}

void InitializeDut::displayLCDError(const char* message, uint32 errorno)
{
    char ShortDescription[MAX_LCD_SHORTDESC], LongDescription[MAX_LCD_LONGDESC];

    logger_.log(Logger::ERR,"LCD ERROR: %s %d", message, errorno);

    memset(LongDescription, 0x00, MAX_LCD_LONGDESC);
    memset(ShortDescription, 0x00, MAX_LCD_SHORTDESC);
    GetLoaderErrorDescription(errorno, (uint8 *)ShortDescription, (uint8 *)LongDescription, MAX_LCD_SHORTDESC, MAX_LCD_LONGDESC);
    if (strlen(LongDescription) != 0)
       logger_.log(Logger::ERR, "LCD ERROR %d : %s", errorno, LongDescription);

    return;
}

/* @} */
