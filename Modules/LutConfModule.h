#ifndef LUTCONFMODULE_H
#define LUTCONFMODULE_H

#include <memory>
#include <functional>
#include <unordered_map>
#include <utility>

//#include "MemoryAdapter.h"
#include "IDaemonModule.h"

class MemoryAdapter;

class LutConfModule : public IDaemonModule
{
    uint32_t address;
    uint32_t memorySize;

    std::shared_ptr<MemoryAdapter> _memoryAdapter;

    bool SetLut(std::vector<uint8_t>& lutBuffer, std::string& message);
    bool GetLut(std::vector<uint8_t>& lutBuffer, std::string& message);
    void ParseValues (std::vector<uint8_t>& lutBuffer, std::vector<int32_t>& values, std::string& message);
    
public:
	LutConfModule();

	~LutConfModule();

private:
	void RegisterAvailableMethods();

};
#endif // LUTCONFMODULE_H
