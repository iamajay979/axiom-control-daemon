#ifndef I2CHELPERMODULE_H
#define I2CHELPERMODULE_H

#include <vector>
#include <memory>
#include "IDaemonModule.h"


class I2CAdapter;

class I2CHelperModule : public IDaemonModule
{
	std::shared_ptr<I2CAdapter> _i2cAdapter;

public:
	I2CHelperModule();

	bool SetI2C0(const std::string chipAddress, const std::string dataAddress, std::string value, const std::string mode, std::string& message);
	bool SetI2C1(const std::string chipAddress, const std::string dataAddress, std::string value, const std::string mode, std::string& message);
	bool SetI2C2(const std::string chipAddress, const std::string dataAddress, std::string value, const std::string mode, std::string& message);
	
	bool GetI2C0(const std::string chipAddress, const std::string dataAddress, std::string &value, const std::string mode, std::string& message);
	bool GetI2C1(const std::string chipAddress, const std::string dataAddress, std::string &value, const std::string mode, std::string& message);
	bool GetI2C2(const std::string chipAddress, const std::string dataAddress, std::string &value, const std::string mode, std::string& message);

	~I2CHelperModule();
protected:
	void RegisterAvailableMethods();
};

#endif