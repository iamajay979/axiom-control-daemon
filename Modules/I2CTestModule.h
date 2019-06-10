#ifndef I2CTESTMODULE_H
#define I2CTESTMODULE_H

#include "IDaemonModule.h"

class I2CAdapter;

class I2CTestModule : public IDaemonModule
{
	std::shared_ptr<I2CAdapter> _i2cAdapter;

	bool GetPac1720Info(std::string &pac1720Info, std::string &message);
	bool SetPac1720Info(std::string pac1720Info, std::string, std::string& message);
	
public:
	I2CTestModule();

	~I2CTestModule();
protected:
	void RegisterAvailableMethods();
}
#endif