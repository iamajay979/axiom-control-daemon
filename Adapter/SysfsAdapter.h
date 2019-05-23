#ifndef SYSFSADAPTER_H
#define SYSFSADAPTER_H

#include <memory>
#include <functional>
#include <unordered_map>
#include <vector>
#include <utility>
#include <glob.h>

#include "IDaemonModule.h"

class MemoryAdapter;

class SysfsAdapter : public IDaemonModule
{	

    bool SetZynqInfo(std::string zynqInfo, std::string, std::string& message);
	bool GetZynqInfo(std::string& zynqInfo, std::string& message);


public:
	SysfsAdapter();

	~SysfsAdapter();
protected:
	void RegisterAvailableMethods();
	void ReadFile(std::string filename,std::string &message);
	void Glob(const std::string filename,std::vector<std::string> &filenames ,std::string &message);

};

#endif