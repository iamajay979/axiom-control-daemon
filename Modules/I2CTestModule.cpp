#include "I2CTestModule.h"

#include "../Adapters/I2CAdapter.h"

#define GETTER_FUNC(A) std::bind(A, this, std::placeholders::_1, std::placeholders::_2)
#define SETTER_FUNC(A) std::bind(A, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)

I2CTestModule::I2CTestModule() :
	_i2cAdapter(std::make_shared<I2CAdapter>())
{
	RegisterAvailableMethods();
}

void I2CTestModule::RegisterAvailableMethods()
{
	AddParameterHandler("pac1720info", GETTER_FUNC(&I2CTestModule::GetPac1720Info), SETTER_FUNC(&I2CTestModule::SetInfo));
	AddParameterHandler("ZED_5V", GETTER_FUNC(&I2CTestModule::GetZED_5Vinfo), SETTER_FUNC(&I2CTestModule::SetInfo));
	AddParameterHandler("HDN", GETTER_FUNC(&I2CTestModule::GetHDNinfo), SETTER_FUNC(&I2CTestModule::SetInfo));
	AddParameterHandler("HDS", GETTER_FUNC(&I2CTestModule::GetHDSinfo), SETTER_FUNC(&I2CTestModule::SetInfo));
	AddParameterHandler("RFW_V", GETTER_FUNC(&I2CTestModule::GetRFW_Vinfo), SETTER_FUNC(&I2CTestModule::SetInfo));
	AddParameterHandler("RFE_V", GETTER_FUNC(&I2CTestModule::GetRFE_Vinfo), SETTER_FUNC(&I2CTestModule::SetInfo));
	AddParameterHandler("VCCO_35", GETTER_FUNC(&I2CTestModule::GetVCCO_35info), SETTER_FUNC(&I2CTestModule::SetInfo));
	AddParameterHandler("PCIE_IO", GETTER_FUNC(&I2CTestModule::GetPCIE_IOinfo), SETTER_FUNC(&I2CTestModule::SetInfo));
	AddParameterHandler("W_VW", GETTER_FUNC(&I2CTestModule::GetW_VWinfo), SETTER_FUNC(&I2CTestModule::SetInfo));
	AddParameterHandler("N_VN", GETTER_FUNC(&I2CTestModule::GetN_VNinfo), SETTER_FUNC(&I2CTestModule::SetInfo));
	AddParameterHandler("E_VE", GETTER_FUNC(&I2CTestModule::GetE_VEinfo), SETTER_FUNC(&I2CTestModule::SetInfo));
	AddParameterHandler("S_VS", GETTER_FUNC(&I2CTestModule::GetS_VSinfo), SETTER_FUNC(&I2CTestModule::SetInfo));
	AddParameterHandler("BETA_5V", GETTER_FUNC(&I2CTestModule::GetBETA_5Vinfo), SETTER_FUNC(&I2CTestModule::SetInfo));
	AddParameterHandler("PCIE_N_V", GETTER_FUNC(&I2CTestModule::GetPCIE_N_Vinfo), SETTER_FUNC(&I2CTestModule::SetInfo));
	AddParameterHandler("PCIE_S_V", GETTER_FUNC(&I2CTestModule::GetPCIE_S_Vinfo), SETTER_FUNC(&I2CTestModule::SetInfo));
	AddParameterHandler("IOW_V", GETTER_FUNC(&I2CTestModule::GetIOW_Vinfo), SETTER_FUNC(&I2CTestModule::SetInfo));
	AddParameterHandler("IOE_V", GETTER_FUNC(&I2CTestModule::GetIOE_Vinfo), SETTER_FUNC(&I2CTestModule::SetInfo));
	AddParameterHandler("VCCO_13", GETTER_FUNC(&I2CTestModule::GetVCCO_13info), SETTER_FUNC(&I2CTestModule::SetInfo));
	AddParameterHandler("VCCO_34", GETTER_FUNC(&I2CTestModule::GetVCCO_34info), SETTER_FUNC(&I2CTestModule::SetInfo));
	AddParameterHandler("N_VW", GETTER_FUNC(&I2CTestModule::GetN_VWinfo), SETTER_FUNC(&I2CTestModule::SetInfo));
	AddParameterHandler("N_VE", GETTER_FUNC(&I2CTestModule::GetN_VEinfo), SETTER_FUNC(&I2CTestModule::SetInfo));
	AddParameterHandler("S_VE", GETTER_FUNC(&I2CTestModule::GetS_VEinfo), SETTER_FUNC(&I2CTestModule::SetInfo));
	AddParameterHandler("S_VW", GETTER_FUNC(&I2CTestModule::GetS_VWinfo), SETTER_FUNC(&I2CTestModule::SetInfo));
}

I2CTestModule::~I2CTestModule()
{

}

bool I2CTestModule::GetZED_5Vinfo(std::string &zed5vInfo, std::string &message)
{	
	zed5vInfo = GetInfo("0x28", "ZED_5V", 1, message);

	return true;
}	
bool I2CTestModule::GetHDNinfo(std::string &hdnInfo, std::string &message)
{
	hdnInfo = GetInfo("0x29", "HDN", 1, message);

	return true;
}

bool I2CTestModule::GetHDSinfo(std::string &hdsInfo, std::string &message)
{
	hdsInfo = GetInfo("0x2a", "HDS", 1, message);

	return true;
}

bool I2CTestModule::GetRFW_Vinfo(std::string &rfwvInfo, std::string &message)
{
	rfwvInfo = GetInfo("0x2c", "RFW_V", 1, message);

	return true;
}

bool I2CTestModule::GetRFE_Vinfo(std::string &rfevInfo, std::string &message)
{
	rfevInfo = GetInfo("0x2d", "RFE_V", 1, message);

	return true;
}

bool I2CTestModule::GetVCCO_35info(std::string &vcco35Info, std::string &message)
{
	vcco35Info = GetInfo("0x48", "VCCO_35", 1, message);

	return true;
}

bool I2CTestModule::GetPCIE_IOinfo(std::string &pcieioInfo, std::string &message)
{
	pcieioInfo = GetInfo("0x49", "PCIE_IO", 1, message);

	return true;
}

bool I2CTestModule::GetW_VWinfo(std::string &wvwInfo, std::string &message)
{
	wvwInfo = GetInfo("0x4a", "W_VW", 1, message);

	return true;
}

bool I2CTestModule::GetN_VNinfo(std::string &nvnInfo, std::string &message)
{
	nvnInfo = GetInfo("0x4b", "N_VN", 1, message);

	return true;
}

bool I2CTestModule::GetE_VEinfo(std::string &eveInfo, std::string &message)
{
	eveInfo = GetInfo("0x4c", "E_VE", 1, message);

	return true;
}

bool I2CTestModule::GetS_VSinfo(std::string &svsInfo, std::string &message)
{
	svsInfo = GetInfo("0x4d", "S_VS", 1, message);

	return true;
}

bool I2CTestModule::GetBETA_5Vinfo(std::string &beta5vInfo, std::string &message)
{
	beta5vInfo = GetInfo("0x28", "BETA_5V", 0, message);

	return true;
}

bool I2CTestModule::GetPCIE_N_Vinfo(std::string &pcienvInfo, std::string &message)
{
	pcienvInfo = GetInfo("0x29", "PCIE_N_V", 0, message);

	return true;
}

bool I2CTestModule::GetPCIE_S_Vinfo(std::string &pciesvInfo, std::string &message)
{
	pciesvInfo = GetInfo("0x2a", "PCIE_S_V", 0, message);

	return true;
}

bool I2CTestModule::GetIOW_Vinfo(std::string &iowvInfo, std::string &message)
{
	iowvInfo = GetInfo("0x2c", "IOW_V", 0, message);

	return true;
}

bool I2CTestModule::GetIOE_Vinfo(std::string &ioevInfo, std::string &message)
{
	ioevInfo = GetInfo("0x2d", "IOE_V", 0, message);

	return true;
}

bool I2CTestModule::GetVCCO_13info(std::string &vcco13Info, std::string &message)
{
	vcco13Info = GetInfo("0x48", "VCCO_13", 0, message);

	return true;
}

bool I2CTestModule::GetVCCO_34info(std::string &vcco34Info, std::string &message)
{
	vcco34Info = GetInfo("0x49", "VCCO_34", 0, message);

	return true;
}

bool I2CTestModule::GetN_VWinfo(std::string &nvwInfo, std::string &message)
{
	nvwInfo = GetInfo("0x4a", "N_VW", 0, message);

	return true;
}

bool I2CTestModule::GetN_VEinfo(std::string &nveInfo, std::string &message)
{
	nveInfo = GetInfo("0x4b", "N_VE", 0, message);

	return true;
}

bool I2CTestModule::GetS_VEinfo(std::string &sveInfo, std::string &message)
{
	sveInfo = GetInfo("0x4c", "S_VE", 0, message);

	return true;
}

bool I2CTestModule::GetS_VWinfo(std::string &svwInfo, std::string &message)
{
	svwInfo = GetInfo("0x4d", "S_VW", 0, message);

	return true;
}

std::string I2CTestModule::GetInfo(const std::string id, const std::string sensorName, int flag, std::string &message)
{
	int res;
	double voltageSource;
	double voltageSense; 
	double current;
	std::string info;
	char buff[200];

	res = TestI2C(id, message);
	if(res < 0)
	{	
		message = "Error : I2CModule failed to get value";
		return message;
	}

	SetSampleTime(id, message);

	if (flag)
	{
		voltageSource = GetVoltageSource(id, 1, message);
		voltageSense = GetVoltageSense(id, 1, message);
	}
	else
	{
		voltageSource = GetVoltageSource(id, 0, message);
		voltageSense = GetVoltageSense(id, 0, message);
	}
	current = GetCurrent(voltageSense);

	sprintf(buff,"%-14.14s\t%6.4f V\t%+8.4f mV  %+8.2f mA\n",sensorName.c_str(),voltageSource,voltageSense ,current);
	info = buff;

	return info;
}

double I2CTestModule::GetVoltageSource(const std::string id, int flag, std::string &message)
{
	int voltageSourceHex;
	double voltageSource;

	if(flag)
	{
		voltageSourceHex = ((_i2cAdapter->I2cGet("0", id, "0x11", 'b', message) << 8) | (_i2cAdapter->I2cGet("0", id, "0x12", 'b', message)));
	}
	else
	{
		voltageSourceHex = ((_i2cAdapter->I2cGet("0", id, "0x13", 'b', message) << 8) | (_i2cAdapter->I2cGet("0" ,id, "0x14", 'b', message)));
	}

	voltageSource = voltageSourceHex * 20 / static_cast<double>(32768);

	return voltageSource;
}

double I2CTestModule::GetVoltageSense(const std::string id, int flag, std::string &message)
{
	int voltageSenseHex, temp;
	double voltageSense;

	if(flag)
	{
		voltageSenseHex = ((_i2cAdapter->I2cGet("0", id, "0x0D", 'b', message) << 4) | (_i2cAdapter->I2cGet("0", id, "0x0E", 'b', message) >> 4));
	}
	else
	{
		voltageSenseHex = ((_i2cAdapter->I2cGet("0", id, "0x0F", 'b', message) << 4) | (_i2cAdapter->I2cGet("0", id, "0x10", 'b', message) >> 4));
	}

	if(voltageSenseHex >= 2048)
	{
		temp = voltageSenseHex - 4096;
	}
	else
	{
		temp = voltageSenseHex;
	}

	voltageSense = temp * 80 / static_cast<double>(2048);

	return voltageSense;
}

double I2CTestModule::GetCurrent(double voltageSense)
{
	return voltageSense / 0.020;
}

void I2CTestModule::SetSampleTime(const std::string id, std::string &message)
{	
	int res;

	res = _i2cAdapter->I2cSet("0", id, "0x0A", "0xFF", 'b', message);
	res = _i2cAdapter->I2cSet("0", id, "0x0B", "0xFF", 'b', message);
	res = _i2cAdapter->I2cSet("0", id, "0x0C", "0xFF", 'b', message);
}

int I2CTestModule::TestI2C(const std::string id, std::string &message)
{	
	int res;

	res = _i2cAdapter->I2cGet("0", id, "-1", 'b', message);
	return res;
}

bool I2CTestModule::GetPac1720Info(std::string &pac1720Info, std::string &message)
{	

	std::string info;

	GetZED_5Vinfo(info, message);
	pac1720Info += info;
	GetBETA_5Vinfo(info, message);
	pac1720Info += info;

	GetHDNinfo(info, message);
	pac1720Info += info;
	GetPCIE_N_Vinfo(info, message);
	pac1720Info += info;

	GetHDSinfo(info, message);
	pac1720Info += info;
	GetPCIE_S_Vinfo(info, message);
	pac1720Info += info;

	GetRFW_Vinfo(info, message);
	pac1720Info += info;
	GetIOW_Vinfo(info, message);
	pac1720Info += info;

	GetRFE_Vinfo(info, message);
	pac1720Info += info;
	GetIOE_Vinfo(info, message);
	pac1720Info += info;

	GetVCCO_35info(info, message);
	pac1720Info += info;
	GetVCCO_13info(info, message);
	pac1720Info += info;

	GetPCIE_IOinfo(info, message);
	pac1720Info += info;
	GetVCCO_34info(info, message);
	pac1720Info += info;

	GetW_VWinfo(info, message);
	pac1720Info += info;
	GetN_VWinfo(info, message);
	pac1720Info += info;

	GetN_VNinfo(info, message);
	pac1720Info += info;
	GetN_VEinfo(info, message);
	pac1720Info += info;

	GetE_VEinfo(info, message);
	pac1720Info += info;
	GetS_VEinfo(info, message);
	pac1720Info += info;

	GetS_VSinfo(info, message);
	pac1720Info += info;
	GetS_VWinfo(info, message);
	pac1720Info += info;

	return true;
}

bool I2CTestModule::SetInfo(std::string pac1720Info, std::string, std::string& message)
{	

	UNUSED(pac1720Info);

	message = "Set Method Not Allowed for the given parameter ";

	return true;
}
