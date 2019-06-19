#ifndef ENVIRONMENTMODULE_H
#define ENVIRONMENTMODULE_H

#include <memory>
#include <functional>
#include <unordered_map>
#include <vector>
#include <utility>
#include <glob.h>

#include "IDaemonModule.h"

class SysfsAdapter;

class EnvironmentModule : public IDaemonModule
{	
	std::shared_ptr<SysfsAdapter> _sysfsAdapter;

    bool SetZynqTemp(std::string temperature, std::string, std::string& message);
	bool GetZynqTemp(std::string& temperature, std::string& message);
	bool SetZynqVoltage(std::string voltage, std::string, std::string& message);
	bool GetZynqVoltage(std::string& voltage, std::string& message);
	bool GetVccint(std::string& voltage, std::string& message);
	bool GetVccaux(std::string& voltage, std::string& message);
	bool GetVccbram(std::string& voltage, std::string& message);
	bool GetVccpint(std::string& voltage, std::string& message);
	bool GetVccpaux(std::string& voltage, std::string& message);
	bool GetVccoddr(std::string& voltage, std::string& message);
	bool GetVrefp(std::string& voltage, std::string& message);
	bool GetVrefn(std::string& voltage, std::string& message);
	void GetVoltage(const std::string voltageRaw, const std::string voltageScale, std::string& voltage, std::string& message);

public:
	EnvironmentModule();

	~EnvironmentModule();
protected:
	void RegisterAvailableMethods();
};

#endif