#ifndef I2CADAPTER_H
#define I2CADAPTER_H

#include <fstream>
#include <sys/syslog.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <dirent.h>
#include <fcntl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <iomanip>
#include <sstream>
 
// #include <json/json.hpp>
// using json = nlohmann::json;

#include "IAdapter.h"

#include "../Helpers/Helpers.h"

class I2CAdapter : public IAdapter
{
    int Openi2cDev(int i2cbus, std::string &message);
    int SetSlaveAddr(int file, int chipAddress, std::string &message);
    int StrToInt(std::string hexStr);
    int SetAddressPointer(int busFd, char *buff, int size);
    int WriteRegister(int busFd, int dataAddress, int value, int size, std::string &message);
    int ReadRegister(int busFd, int dataAddress, int size, std::string &message);

public:
    I2CAdapter();
    
    void ReadDescriptions(std::string descriptionFile) override
    {
        std::ifstream in(descriptionFile);
        if(!in.is_open())
        {
            std::string errorMessage = "Description file " + descriptionFile + " not found.";
            syslog (LOG_ERR, "%s", errorMessage.c_str());
            return;
        }
        //json j;
        //in >> j;
    }

    void CheckDevices() override {}

    void ReadByte(uint8_t data) override;
    void WriteByte(uint8_t data) override;
    void ReadBlock(uint8_t *data, unsigned int length) override;
    void WriteBlock(uint8_t *data, unsigned int length) override;

    int I2cGet(std::string i2cbusArg, std::string chipAddressArg, std::string dataAddressArg, char mode, std::string &message);
    int I2cSet(std::string i2cbusArg, std::string chipAddressArg, std::string dataAddressArg, std::string valueArg, char mode, std::string &message);

    void Execute() override;

    std::vector<std::string> GetAvailableMethods()
    {
        std::vector<std::string> availableMethods;
        availableMethods.push_back("Test1");
        availableMethods.push_back("Test2");

        return availableMethods;
    }
};

#endif //I2CADAPTER_H
