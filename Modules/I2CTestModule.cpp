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
	AddParameterHandler("pac1720info", GETTER_FUNC(&I2CTestModule::GetPac1720Info), SETTER_FUNC(&I2CTestModule::SetPac1720Info));
}

I2CTestModule::~I2CTestModule()
{

}

bool I2CTestModule::GetPac1720Info(std::string &pac1720Info, std::string &message)
{
	std::vector<std::string> PACn{"0x28", "0x29", "0x2a", "0x2c", "0x2d", "0x48", "0x49", "0x4a", "0x4b", "0x4c", "0x4d"}; 
	std::vector<float> VM{20, 10, 5, 2.5, 1.25, 0.625, 0.3125, 0.1563, 0.0781, 0.0390, 0.0195};
	std::vector<float> MR1v{0.020, 0.020, 0.020, 0.020, 0.020, 0.020, 0.020, 0.020, 0.020, 0.020, 0.020};
	std::vector<float> MR2v{0.020, 0.020, 0.020, 0.020, 0.020, 0.020, 0.020, 0.020, 0.020, 0.020, 0.020};
	std::vector<std::string> VS1l{"ZED_5V", "HDN", "HDS", "RFW_V", "RFE_V", "VCCO_35", "PCIE_IO", "W_VW", "N_VN", "E_VE", "S_VS"};
	std::vector<std::string> VS2l{"BETA_5V", "PCIE_N_V", "PCIE_S_V", "IOW_V", "IOE_V", "VCCO_13", "VCCO_34", "N_VW", "N_VE", "S_VE", "S_VW"};

	int res;
	int VV1i,VV2i,VV1f,VV2f,VS1i,VS2i,VS1v,VS2v,count=0;
	float VS1f,VS2f,VA1f,VA2f;

	for(auto& it : PACn)
	{	

		std::string ID = it;
		res = _i2cAdapter->I2cGet("0",ID,"-1",'b',message);
		if(res < 0)
		{
			continue;
		}
		res = _i2cAdapter->I2cSet("0",ID,"0x0A","0xFF",'b',message);
		res = _i2cAdapter->I2cSet("0",ID,"0x0B","0xFF",'b',message);
		res = _i2cAdapter->I2cSet("0",ID,"0x0C","0xFF",'b',message);

		VV1i = ((_i2cAdapter->I2cGet("0",ID,"0x11",'b',message) << 8) | (_i2cAdapter->I2cGet("0",ID,"0x12",'b',message)));
		VV2i = ((_i2cAdapter->I2cGet("0",ID,"0x13",'b',message) << 8) | (_i2cAdapter->I2cGet("0",ID,"0x14",'b',message)));

		VV1f = VV1i * 20 / 32768;
		VV2f = VV2i * 20 / 32768;

		VS1i = ((_i2cAdapter->I2cGet("0",ID,"Ox0D",'b',message) << 4) | (_i2cAdapter->I2cGet("0",ID,"0x0E",'b',message) >> 4));
		VS2i = ((_i2cAdapter->I2cGet("0",ID,"Ox0F",'b',message) << 4) | (_i2cAdapter->I2cGet("0",ID,"0x10",'b',message) >> 4));

		if(VS1i >= 2048)
		{
			VS1v = VS1i - 4096;
		}
		else
		{
			VS1v = VS1i;
		}

		if(VS2i >= 2048)
		{
			VS2v = VS2i - 4096;
		}
		else
		{
			VS2v = VS2i;
		}

		VS1f = VS1v * 80 / 2048;
		VS2f = VS2v * 80 / 2048;

		VA1f = VS1f / MR1v[count];
		VA2f = VS2f / MR2v[count];

		count++;

		message = (std::to_string(VV1f) + " " +std::to_string(VV1i) + " " + std::to_string(VS1f) + " " + std::to_string(VS1i) + " " + std::to_string(VA1f) + "\n");
		message += (std::to_string(VV2f) + " " +std::to_string(VV2i) + " " + std::to_string(VS2f) + " " + std::to_string(VS2i) + " " + std::to_string(VA2f));
	}
	pac1720Info = message;

	return true;
}

bool I2CTestModule::SetPac1720Info(std::string pac1720Info, std::string, std::string& message)
{	

	UNUSED(pac1720Info);

	message = "Set Method Not Allowed for the given parameter ";

	return true;
}