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
	AddParameterHandler("voltage_all", GETTER_FUNC(&EnvironmentModule::GetZynqVoltage), SETTER_FUNC(&EnvironmentModule::SetZynqVoltage));
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

	std::string zynqTempOffset;
	std::string zynqTempRaw;
	std::string zynqTempScale;
	float zynqTemp;
	std::vector<std::string> filenames;


	_sysfsAdapter->Glob("/sys/devices/soc0/amba/*.adc/iio*/in_temp0_offset", filenames, message);
	_sysfsAdapter->ReadFile(filenames[0], zynqTempOffset);
	_sysfsAdapter->Glob("/sys/devices/soc0/amba/*.adc/iio*/in_temp0_raw", filenames, message);
	_sysfsAdapter->ReadFile(filenames[1], zynqTempRaw);
	_sysfsAdapter->Glob("/sys/devices/soc0/amba/*.adc/iio*/in_temp0_scale", filenames, message);
	_sysfsAdapter->ReadFile(filenames[2], zynqTempScale);

	zynqTemp = ( std::stod(zynqTempRaw) + std::stod(zynqTempOffset) ) * std::stod(zynqTempScale) / 1000 ;

	temperature = std::to_string(zynqTemp);

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
	std::string voltageRaw;
	std::string voltageScale;

	voltageRaw = "/sys/devices/soc0/amba/*.adc/iio*/in_voltage0_vccint_raw";
	voltageScale = "/sys/devices/soc0/amba/*.adc/iio*/in_voltage0_vccint_scale";

	GetVoltage(voltageRaw, voltageScale, voltage, message);

	return true;
}

bool EnvironmentModule::GetVccaux(std::string& voltage, std::string& message)
{
	std::string voltageRaw;
	std::string voltageScale;

	voltageRaw = "/sys/devices/soc0/amba/*.adc/iio*/in_voltage1_vccaux_raw";
	voltageScale = "/sys/devices/soc0/amba/*.adc/iio*/in_voltage1_vccaux_scale";

	GetVoltage(voltageRaw, voltageScale, voltage, message);
	
	return true;

}

bool EnvironmentModule::GetVccbram(std::string& voltage, std::string& message)
{
	std::string voltageRaw;
	std::string voltageScale;

	voltageRaw = "/sys/devices/soc0/amba/*.adc/iio*/in_voltage2_vccbram_raw";
	voltageScale = "/sys/devices/soc0/amba/*.adc/iio*/in_voltage2_vccbram_scale";

	GetVoltage(voltageRaw, voltageScale, voltage, message);
	
	return true;
}

bool EnvironmentModule::GetVccpint(std::string& voltage, std::string& message)
{
	std::string voltageRaw;
	std::string voltageScale;

	voltageRaw = "/sys/devices/soc0/amba/*.adc/iio*/in_voltage3_vccpint_raw";
	voltageScale = "/sys/devices/soc0/amba/*.adc/iio*/in_voltage3_vccpint_scale";

	GetVoltage(voltageRaw, voltageScale, voltage, message);
	
	return true;
}

bool EnvironmentModule::GetVccpaux(std::string& voltage, std::string& message)
{
	std::string voltageRaw;
	std::string voltageScale;

	voltageRaw = "/sys/devices/soc0/amba/*.adc/iio*/in_voltage4_vccpaux_raw";
	voltageScale = "/sys/devices/soc0/amba/*.adc/iio*/in_voltage4_vccpaux_scale";

	GetVoltage(voltageRaw, voltageScale, voltage, message);
	
	return true;
}

bool EnvironmentModule::GetVccoddr(std::string& voltage, std::string& message)
{
	std::string voltageRaw;
	std::string voltageScale;

	voltageRaw = "/sys/devices/soc0/amba/*.adc/iio*/in_voltage5_vccoddr_raw";
	voltageScale = "/sys/devices/soc0/amba/*.adc/iio*/in_voltage5_vccoddr_scale";

	GetVoltage(voltageRaw, voltageScale, voltage, message);
	
	return true;
}

bool EnvironmentModule::GetVrefp(std::string& voltage, std::string& message)
{
	std::string voltageRaw;
	std::string voltageScale;

	voltageRaw = "/sys/devices/soc0/amba/*.adc/iio*/in_voltage6_vrefp_raw";
	voltageScale = "/sys/devices/soc0/amba/*.adc/iio*/in_voltage6_vrefp_scale";

	GetVoltage(voltageRaw, voltageScale, voltage, message);
	
	return true;
}

bool EnvironmentModule::GetVrefn(std::string& voltage, std::string& message)
{
	std::string voltageRaw;
	std::string voltageScale;

	voltageRaw = "/sys/devices/soc0/amba/*.adc/iio*/in_voltage7_vrefn_raw";
	voltageScale = "/sys/devices/soc0/amba/*.adc/iio*/in_voltage7_vrefn_scale";

	GetVoltage(voltageRaw, voltageScale, voltage, message);
	
	return true;
}

void EnvironmentModule::GetVoltage(const std::string voltageRaw, const std::string voltageScale, std::string& voltage ,std::string& message)
{
	std::string zynqVoltageRaw;
	std::string zynqVoltageScale;
	float zynqVoltage;
	std::vector<std::string> filenames;

	_sysfsAdapter->Glob(voltageRaw, filenames, message);//returns a vector of paths found
	_sysfsAdapter->ReadFile(filenames[0], zynqVoltageRaw);
	_sysfsAdapter->Glob(voltageScale, filenames, message);
	_sysfsAdapter->ReadFile(filenames[1], zynqVoltageScale);

	zynqVoltage = std::stod(zynqVoltageRaw) * std::stod(zynqVoltageScale) / 1000;
	voltage = std::to_string(zynqVoltage);
}

//Test function to get all the voltages
bool EnvironmentModule::GetZynqVoltage(std::string& voltage, std::string& message)
{	
	UNUSED(voltage);

	std::string zynqVoltageName;
	std::string zynqVoltageRaw;
	std::string zynqVoltageScale;
	std::string temp1,temp2;
	float zynqVoltage;
	std::vector<std::string> filenames;

	_sysfsAdapter->Glob("/sys/devices/soc0/amba/*.adc/iio*/in_voltage*raw", filenames, message);

	for(auto it : filenames)
	{	
		temp1 = it.substr(0, it.length() - 4);
		std::size_t pos = temp1.rfind("_v");
		zynqVoltageName = temp1.substr(pos + 1);
		zynqVoltageName[0] = 'V';
		_sysfsAdapter->ReadFile(it, zynqVoltageRaw);

		temp2 = temp1.append("_scale");
		_sysfsAdapter->ReadFile(temp2, zynqVoltageScale);

		zynqVoltage = std::stod(zynqVoltageRaw) * std::stod(zynqVoltageScale) / 1000;

		voltage += ('\n' + zynqVoltageName + '\t' + std::to_string(zynqVoltage) + " V");
	}

	return true;
}

bool EnvironmentModule::SetZynqVoltage(std::string voltage, std::string, std::string& message)
{
	UNUSED(voltage);

	message = "Set Method Not Allowed for the given parameter ";

	return true;
}


