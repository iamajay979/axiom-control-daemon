#include "I2CAdapter.h"

I2CAdapter::I2CAdapter()
{
}

void I2CAdapter::ReadByte(uint8_t data)
{
    UNUSED(data);
}

void I2CAdapter::WriteByte(uint8_t data)
{
    UNUSED(data);
}

void I2CAdapter::ReadBlock(uint8_t *data, unsigned int length)
{
    UNUSED(data);
    UNUSED(length);
}

void I2CAdapter::WriteBlock(uint8_t *data, unsigned int length)
{
    UNUSED(data);
    UNUSED(length);
}

void I2CAdapter::Execute() {}

void I2CAdapter::I2cGet(std::string i2cbusArg, std::string chipAddressArg,std::string dataAddressArg, std::string mode ,std::string &message);
{	
	//Assumption is that the i2cbus is a valid integer, if names of i2cbus has to incorporated then a proper lookup function has to created similar to system func "i2cget"
	unsigned long i2cbus,chipAddress,dataAddress;
	int file;

	try
	{
		i2cbus = stoul(i2cbus,nullptr,0);
	}
	catch(const std::invalid_argument& ia)
	{
		message = "Invalid argument : " + std::string(ia.what());
		return ;
	}
	if (i2cbus > 0xFFFFF)
	{
		message = "Error : I2C bus out of range ";
		return;
	}

	try
	{
		chipAddress = stoul(chipAddress,nullptr,0);
	}
	catch(const std::invalid_argument& ia)
	{
		message = "Invalid argument : " + std::string(ia.what());
		return ;	
	}
	if(chipAddress < 0x03 || chipAddress > 0x77) //Replace them with const var ?
	{
		message = " Error : Chip Address out of range ";
		return ;
	}

	try
	{
		dataAddress = stoul(dataAddressArg,nullptr,0);	
	}
	catch(const std::invalid_argument& ia)
	{
		message = "Invalid argument : " + std::string(ia.what());
		return ;
	}
	if(dataAddress < 0 || dataAddress > 0xff)
	{
		message = "Error : Data Address out of range";
	}

	file = Openi2cDev(i2cbus,filename,message);

	if(file < 0)
	{	
		message = ("Error : Could not open file " + "dev/i2c-" + std::to_string(i2cbus));
		return;
	}
	SetSlaveAddr(file, chipAddress, message);


}

int I2CAdapter::Openi2cDev(int i2cbus, std::string filename, std::string &message)
{
	int file;
	filename = "dev/i2c-" + std::to_string(i2cbus);
	file = open(filename ,O_RDWR);
	//Add further error handling if required
}

int I2CAdapter::SetSlaveAddr(int file , int chipAddress ,std::string &message) // if required add force flag in this function 
{	
	std::stringstream ss;
	if(ioctl(file, I2C_SLAVE, chipAddress) < 0)
	{	
		ss << std::hex << std::setw(2) << std::setfill('0') << chipAddress ; //to return error message address in hex format
		message = "Could not set address to 0x" + ss.str(); 
	}
}