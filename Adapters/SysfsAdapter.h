#ifndef SYSFSADAPTER_H
#define SYSFSADAPTER_H

#include <memory>
#include <functional>
#include <unordered_map>
#include <vector>
#include <utility>
#include <glob.h>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>

#include "IAdapter.h"

#include "../Helpers/Helpers.h"

class SysfsAdapter : public IAdapter
{
public:
    SysfsAdapter();

    void ReadDescriptions(std::string descriptionFile) override
    {
        UNUSED(descriptionFile);
    }
    void CheckDevices()  override {}

    void ReadByte(uint8_t data) override;
    void WriteByte(uint8_t data) override;
    void ReadBlock(uint8_t *data, unsigned int length) override;
    void WriteBlock(uint8_t *data, unsigned int length) override;

    void Execute() override;

    void ReadFile(std::string filename,std::string &message);
    void Glob(const std::string filename,std::vector<std::string> &filenames ,std::string &message);
};
#endif // SYSFSADAPTER_H
