#ifndef I2CTESTMODULE_H
#define I2CTESTMODULE_H

#include <vector>
#include <memory>
#include "IDaemonModule.h"


class I2CAdapter;

class I2CTestModule : public IDaemonModule
{
	std::shared_ptr<I2CAdapter> _i2cAdapter;

	bool GetPac1720Info(std::string &pac1720Info, std::string &message);

	bool GetZED_5Vinfo(std::string &zed5vInfo, std::string &message);
	bool GetHDNinfo(std::string &hdnInfo, std::string &message);
	bool GetHDSinfo(std::string &hdsInfo, std::string &message);
	bool GetRFW_Vinfo(std::string &rfwvInfo, std::string &message);
	bool GetRFE_Vinfo(std::string &rfevInfo, std::string &message);	
	bool GetVCCO_35info(std::string &vcco35Info, std::string &message);
	bool GetPCIE_IOinfo(std::string &pcieioInfo, std::string &message);
	bool GetW_VWinfo(std::string &wvwInfo, std::string &message);
	bool GetN_VNinfo(std::string &nvnInfo, std::string &message);
	bool GetE_VEinfo(std::string &eveInfo, std::string &message);
	bool GetS_VSinfo(std::string &svsInfo, std::string &message);
	bool GetBETA_5Vinfo(std::string &beta5vInfo, std::string &message);
	bool GetPCIE_N_Vinfo(std::string &pcienvInfo, std::string &message);
	bool GetPCIE_S_Vinfo(std::string &pciesvInfo, std::string &message);
	bool GetIOW_Vinfo(std::string &iowvInfo, std::string &message);
	bool GetIOE_Vinfo(std::string &ioevInfo, std::string &message);
	bool GetVCCO_13info(std::string &vcco13Info, std::string &message);
	bool GetVCCO_34info(std::string &vcco34Info, std::string &message);
	bool GetN_VWinfo(std::string &nvwInfo, std::string &message);
	bool GetN_VEinfo(std::string &nveInfo, std::string &message);
	bool GetS_VEinfo(std::string &sveInfo, std::string &message);
	bool GetS_VWinfo(std::string &svwInfo, std::string &message);

	bool SetInfo(std::string pac1720Info, std::string, std::string& message);

	//helper functions
	std::string GetInfo(const std::string id, const std::string sensorName, int flag, std::string &message);
	double GetVoltageSource(const std::string id, int flag, std::string &message);
	double GetVoltageSense(const std::string id, int flag, std::string &message);
	double GetCurrent(double voltageSense);
	void SetSampleTime(const std::string id, std::string &message);
	int TestI2C(const std::string id, std::string &message);

	
public:
	I2CTestModule();

	~I2CTestModule();
protected:
	void RegisterAvailableMethods();
};
#endif