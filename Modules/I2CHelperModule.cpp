#include "I2CHelperModule.h"

#include "../Adapters/I2CAdapter.h"

#define GETTER_FUNC(A) std::bind(A, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5)
#define SETTER_FUNC(A) std::bind(A, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5)

I2CHelperModule::I2CHelperModule() :
	_i2cAdapter(std::make_shared<I2CAdapter>())
{
	RegisterAvailableMethods();
}

void I2CHelperModule::RegisterAvailableMethods()
{
	AddI2cParameterHandler("i2c0", GETTER_FUNC(&I2CHelperModule::GetI2C0), SETTER_FUNC(&I2CHelperModule::SetI2C0));
	AddI2cParameterHandler("i2c1", GETTER_FUNC(&I2CHelperModule::GetI2C1), SETTER_FUNC(&I2CHelperModule::SetI2C1));
	AddI2cParameterHandler("i2c2", GETTER_FUNC(&I2CHelperModule::GetI2C2), SETTER_FUNC(&I2CHelperModule::SetI2C2));
}

I2CHelperModule::~I2CHelperModule()
{

}

bool I2CHelperModule::SetI2C0(const std::string chipAddress, const std::string dataAddress, const std::string value, const std::string mode, std::string& message)
{
        int res;
        res = _i2cAdapter->I2cSet("0", chipAddress, dataAddress, value, mode[0], message);

        if(res == -1)
        {
                message = "Set command failed";
                return false;
        }

        return true;
}

bool I2CHelperModule::SetI2C1(const std::string chipAddress, const std::string dataAddress, const std::string value, const std::string mode, std::string& message)
{
        int res;
        res = _i2cAdapter->I2cSet("0", chipAddress, dataAddress, value, mode[0], message);

        if(res == -1)
        {
                message = "Set command failed";
                return false;
        }

        return true;
}

bool I2CHelperModule::SetI2C2(const std::string chipAddress, const std::string dataAddress, const std::string value, const std::string mode, std::string& message)
{
        int res;
        res = _i2cAdapter->I2cSet("2", chipAddress, dataAddress, value, mode[0], message);

        if(res == -1)
        {
                message = "Set command failed";
                return false;
        }

        return true;
}

bool I2CHelperModule::GetI2C0(const std::string chipAddress, const std::string dataAddress, std::string &value, const std::string mode, std::string& message)
{
        int res;
        if(dataAddress == ""){std::cout<<"data address not found" ;return false;}
        res = _i2cAdapter->I2cGet("0", chipAddress, dataAddress, mode[0], message);

        if(res < 0)
        {
                message = "Get command failed";
                return false;
        }

        value = std::to_string(res);
        return true;
}

bool I2CHelperModule::GetI2C1(const std::string chipAddress, const std::string dataAddress, std::string &value, const std::string mode, std::string& message)
{
        int res;
        if(dataAddress == ""){std::cout<<"data address not found" ;return false;}
        res = _i2cAdapter->I2cGet("1", chipAddress, dataAddress, mode[0], message);

        if(res < 0)
        {
                message = "Get command failed";
                return false;
        }

        value = std::to_string(res);
        return true;
}

bool I2CHelperModule::GetI2C2(const std::string chipAddress, const std::string dataAddress, std::string &value, const std::string mode, std::string& message)
{
        int res;
        if(dataAddress == ""){std::cout<<"data address not found" ;return false;}
        res = _i2cAdapter->I2cGet("2", chipAddress, dataAddress, mode[0], message);

        if(res < 0)
        {
                message = "Get command failed";
                return false;
        }

        value = std::to_string(res);
        return true;
}

