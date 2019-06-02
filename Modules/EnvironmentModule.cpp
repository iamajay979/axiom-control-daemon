#include "EnvironmentModule.h"

#include "../Adapters/SysfsAdapter.h"

#define GETTER_FUNC(A) std::bind(A, this, std::placeholders::_1, std::placeholders::_2)
#define SETTER_FUNC(A) std::bind(A, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)

EnvironmentModule::EnvironmentModule() :
	_sysfsAdapter(std::make_shared<SysfsAdapter>())
{
	RegisterAvailableMethods();
}

void EnvironmentModule::RegisterAvailableMethods()
{
	AddParameterHandler("temperature", GETTER_FUNC(&EnvironmentModule::GetZynqTemp), SETTER_FUNC(&EnvironmentModule::SetZynqTemp));
	AddParameterHandler("voltages", GETTER_FUNC(&EnvironmentModule::GetZynqVoltage), SETTER_FUNC(&EnvironmentModule::SetZynqVoltage));
	AddParameterHandler("vccint", GETTER_FUNC(&EnvironmentModule::GetVccint), SETTER_FUNC(&EnvironmentModule::SetZynqVoltage));
	AddParameterHandler("vccaux", GETTER_FUNC(&EnvironmentModule::GetVccaux), SETTER_FUNC(&EnvironmentModule::SetZynqVoltage));
	AddParameterHandler("vccbram", GETTER_FUNC(&EnvironmentModule::GetVccbram), SETTER_FUNC(&EnvironmentModule::SetZynqVoltage));
	AddParameterHandler("vccpint", GETTER_FUNC(&EnvironmentModule::GetVccpint), SETTER_FUNC(&EnvironmentModule::SetZynqVoltage));
	AddParameterHandler("vccpaux", GETTER_FUNC(&EnvironmentModule::GetVccpaux), SETTER_FUNC(&EnvironmentModule::SetZynqVoltage));
	AddParameterHandler("vccoddr", GETTER_FUNC(&EnvironmentModule::GetVccoddr), SETTER_FUNC(&EnvironmentModule::SetZynqVoltage));
	AddParameterHandler("vrefp", GETTER_FUNC(&EnvironmentModule::GetVrefp), SETTER_FUNC(&EnvironmentModule::SetZynqVoltage));
	AddParameterHandler("vrefn", GETTER_FUNC(&EnvironmentModule::GetVrefn), SETTER_FUNC(&EnvironmentModule::SetZynqVoltage));
}

EnvironmentModule::~EnvironmentModule()
{

}

bool EnvironmentModule::GetZynqTemp(std::string& temperature, std::string& message)
{	
	UNUSED(message);

	std::string ZTO,ZTR,ZTS,temp1,temp2,ZVN,ZVR,ZVS;
	float ZT;
	std::vector<std::string> filenames;


	_sysfsAdapter->Glob("/sys/devices/soc0/amba/*.adc/iio*/in_temp0_offset",filenames,message);
	_sysfsAdapter->ReadFile(filenames[0],ZTO);
	_sysfsAdapter->Glob("/sys/devices/soc0/amba/*.adc/iio*/in_temp0_raw",filenames,message);
	_sysfsAdapter->ReadFile(filenames[1],ZTR);
	_sysfsAdapter->Glob("/sys/devices/soc0/amba/*.adc/iio*/in_temp0_scale",filenames,message);
	_sysfsAdapter->ReadFile(filenames[2],ZTS);

	ZT = ( std::stod(ZTR) + std::stod(ZTO) ) * std::stod(ZTS) / 1000 ;

	temperature = std::to_string(ZT);

	return true;
}

bool EnvironmentModule::SetZynqTemp(std::string temperature, std::string, std::string& message)
{
	UNUSED(temperature);

	message = "Set Method Not Allowed for the given parameter ";

	return true;
}

bool EnvironmentModule::GetVccint(std::string& voltage, std::string& message)
{
	std::string voltageRaw ,voltageScale;

	voltageRaw = "/sys/devices/soc0/amba/*.adc/iio*/in_voltage0_vccint_raw";
	voltageScale = "/sys/devices/soc0/amba/*.adc/iio*/in_voltage0_vccint_scale";

	GetVoltage(voltageRaw,voltageScale,voltage,message);

	return true;
}

bool EnvironmentModule::GetVccaux(std::string& voltage, std::string& message)
{
	std::string voltageRaw ,voltageScale;

	voltageRaw = "/sys/devices/soc0/amba/*.adc/iio*/in_voltage1_vccaux_raw";
	voltageScale = "/sys/devices/soc0/amba/*.adc/iio*/in_voltage1_vccaux_scale";

	GetVoltage(voltageRaw,voltageScale,voltage,message);
	
	return true;

}

bool EnvironmentModule::GetVccbram(std::string& voltage, std::string& message)
{
	std::string voltageRaw ,voltageScale;

	voltageRaw = "/sys/devices/soc0/amba/*.adc/iio*/in_voltage2_vccbram_raw";
	voltageScale = "/sys/devices/soc0/amba/*.adc/iio*/in_voltage2_vccbram_scale";

	GetVoltage(voltageRaw,voltageScale,voltage,message);
	
	return true;
}

bool EnvironmentModule::GetVccpint(std::string& voltage, std::string& message)
{
	std::string voltageRaw ,voltageScale;

	voltageRaw = "/sys/devices/soc0/amba/*.adc/iio*/in_voltage3_vccpint_raw";
	voltageScale = "/sys/devices/soc0/amba/*.adc/iio*/in_voltage3_vccpint_scale";

	GetVoltage(voltageRaw,voltageScale,voltage,message);
	
	return true;
}

bool EnvironmentModule::GetVccpaux(std::string& voltage, std::string& message)
{
	std::string voltageRaw ,voltageScale;

	voltageRaw = "/sys/devices/soc0/amba/*.adc/iio*/in_voltage4_vccpaux_raw";
	voltageScale = "/sys/devices/soc0/amba/*.adc/iio*/in_voltage4_vccpaux_scale";

	GetVoltage(voltageRaw,voltageScale,voltage,message);
	
	return true;
}

bool EnvironmentModule::GetVccoddr(std::string& voltage, std::string& message)
{
	std::string voltageRaw ,voltageScale;

	voltageRaw = "/sys/devices/soc0/amba/*.adc/iio*/in_voltage5_vccoddr_raw";
	voltageScale = "/sys/devices/soc0/amba/*.adc/iio*/in_voltage5_vccoddr_scale";

	GetVoltage(voltageRaw,voltageScale,voltage,message);
	
	return true;
}

bool EnvironmentModule::GetVrefp(std::string& voltage, std::string& message)
{
	std::string voltageRaw ,voltageScale;

	voltageRaw = "/sys/devices/soc0/amba/*.adc/iio*/in_voltage6_vrefp_raw";
	voltageScale = "/sys/devices/soc0/amba/*.adc/iio*/in_voltage6_vrefp_scale";

	GetVoltage(voltageRaw,voltageScale,voltage,message);
	
	return true;
}

bool EnvironmentModule::GetVrefn(std::string& voltage, std::string& message)
{
	std::string voltageRaw ,voltageScale;

	voltageRaw = "/sys/devices/soc0/amba/*.adc/iio*/in_voltage7_vrefn_raw";
	voltageScale = "/sys/devices/soc0/amba/*.adc/iio*/in_voltage7_vrefn_scale";

	GetVoltage(voltageRaw,voltageScale,voltage,message);
	
	return true;
}

void EnvironmentModule::GetVoltage(const std::string voltageRaw, const std::string voltageScale, std::string& voltage ,std::string& message)
{
	std::string ZVN,ZVR,ZVS;
	float ZV;
	std::vector<std::string> filenames;

	_sysfsAdapter->Glob(voltageRaw,filenames,message);//returns a vector of paths found
	_sysfsAdapter->ReadFile(filenames[0],ZVR);
	_sysfsAdapter->Glob(voltageScale,filenames,message);
	_sysfsAdapter->ReadFile(filenames[1],ZVS);

	ZV = std::stod(ZVR) * std::stod(ZVS) / 1000;
	voltage = std::to_string(ZV);
}

//Test function to get all the voltages
bool EnvironmentModule::GetZynqVoltage(std::string& voltage, std::string& message)
{	
	UNUSED(voltage);

	std::string temp1,temp2,ZVN,ZVR,ZVS;
	float ZV;
	std::vector<std::string> filenames;
	_sysfsAdapter->Glob("/sys/devices/soc0/amba/*.adc/iio*/in_voltage*raw",filenames,message);

	for(auto it : filenames)
	{	
		temp1 = it.substr(0,it.length() - 4);
		std::size_t pos = temp1.rfind("_v");
		ZVN = temp1.substr(pos + 1);
		ZVN[0] = 'V';

		_sysfsAdapter->ReadFile(it,ZVR);

		temp2 = temp1.append("_scale");
		_sysfsAdapter->ReadFile(temp2,ZVS);

		ZV = std::stod(ZVR) * std::stod(ZVS) / 1000;

		voltage += ('\n' + ZVN + '\t' + std::to_string(ZV) + " V");
	}

	return true;
}

bool EnvironmentModule::SetZynqVoltage(std::string voltage, std::string, std::string& message)
{
	UNUSED(voltage);

	message = "Set Method Not Allowed for the given parameter ";

	return true;
}


