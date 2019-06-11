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

int I2CAdapter::I2cGet(std::string i2cbusArg, std::string chipAddressArg,std::string dataAddressArg, char mode ,std::string &message)
{	
	//Assumption is that the i2cbus is a valid integer, if names of i2cbus has to incorporated then a proper lookup function has to created similar to system func "i2cget"
	//Allowed modes : 'b', 'w', 'c'
	long i2cbus,chipAddress,dataAddress;
	int file, res, size;
	// std::cout<<i2cbusArg<<" "<<chipAddressArg<<" "<<dataAddressArg<<std::endl;
	//can avoid all these try catch by using int and not string params
	try
	{
		i2cbus = std::stol(i2cbusArg,nullptr,0);
	}
	catch(const std::invalid_argument& ia)
	{
		message = "Invalid argument : " + std::string(ia.what());
		return -1;
	}
	if (i2cbus > 0xFFFFF)
	{
		message = "Error : I2C bus out of range ";
		return -1;
	}

	try
	{
		chipAddress = std::stol(chipAddressArg,nullptr,0);
	}
	catch(const std::invalid_argument& ia)
	{
		message = "Invalid argument : " + std::string(ia.what());
		return -1;	
	}
	if(chipAddress < 0x03 || chipAddress > 0x77) //Replace them with const var ?
	{
		message = " Error : Chip Address out of range ";
		return -1;
	}

	try
	{	
		dataAddress = std::stol(dataAddressArg,nullptr,0);	
	}
	catch(const std::invalid_argument& ia)
	{
		message = "Invalid argument : " + std::string(ia.what());
		return -1;
	}
	if(dataAddress > 0xff)//handle negative except -1
	{	
		// std::cout<<dataAddressArg<<std::endl;
		// std::cout<<dataAddress<<std::endl;
		message = "Error : Data Address out of range";
		return -1;
	}

	file = Openi2cDev(i2cbus,message);

	if(file < 0)
	{	
		message = ("Error : Could not open file " + std::string("dev/i2c-") + std::to_string(i2cbus));
		return -1;
	}
	if (SetSlaveAddr(file, chipAddress, message) < 0)
	{
		return -1;
	}

	switch(mode)
	{
		case 'b' :
		{ 
			size = I2C_SMBUS_BYTE_DATA;
			res = i2c_smbus_read_byte_data(file, dataAddress);
			break;
		}
		case 'w' :
		{
			size = I2C_SMBUS_WORD_DATA;
			res = i2c_smbus_read_word_data(file, dataAddress);
			break;
		}
		case 'c' :
		{
			size = I2C_SMBUS_BYTE;
			res = i2c_smbus_write_byte(file, dataAddress);
			if (res < 0)
			{
				message = "Warning - Write Failed ";
				return -1;
			}
			res = i2c_smbus_read_byte(file);
			break;
		}
		default :
		{
			size = I2C_SMBUS_BYTE_DATA;
			res = i2c_smbus_read_word_data(file, dataAddress);
			break;			
		}
	}

	if (res < 0)
	{
		message = "Error : Read Failed ";
		return -1;
	}
	//message  = std::to_string(res);
	return res;
}

int I2CAdapter::I2cSet(std::string i2cbusArg, std::string chipAddressArg,std::string dataAddressArg, std::string valueArg, char mode , std::string &message)
{
	//Flags functionality not added as all scripts only use the 'y' flag ,so all functions assume 'y' is set
	long i2cbus, chipAddress, dataAddress ,value;
	int file, res;

	try
	{
		i2cbus = std::stol(i2cbusArg,nullptr,0);
	}
	catch(const std::invalid_argument& ia)
	{
		message = "Invalid argument : " + std::string(ia.what());
		return -1;
	}
	if (i2cbus > 0xFFFFF)
	{
		message = "Error : I2C bus out of range ";
		return -1;
	}

	try
	{
		chipAddress = std::stol(chipAddressArg,nullptr,0);
	}
	catch(const std::invalid_argument& ia)
	{
		message = "Invalid argument : " + std::string(ia.what());
		return -1;	
	}
	if(chipAddress < 0x03 || chipAddress > 0x77) //Replace them with const var ?
	{
		message = " Error : Chip Address out of range ";
		return -1;
	}

	try
	{
		dataAddress = std::stol(dataAddressArg,nullptr,0);	
	}
	catch(const std::invalid_argument& ia)
	{
		message = "Invalid argument : " + std::string(ia.what());
		return -1;
	}
	if(dataAddress < 0 || dataAddress > 0xff)
	{
		message = "Error : Data Address out of range";
		return -1;
	}

	try
	{
		value = std::stol(valueArg ,nullptr,0);
	}
	catch(const std::invalid_argument& ia)
	{
		message = "Invalid argument : " + std::string(ia.what());
		return -1;
	}
	if(mode == 'b' && (value > 0xFF || value < 0))
	{
		message = "Error: Data value invalid! ";
	}
	if(mode == 'w' && (value > 0xFFFFF || value < 0))
	{
		message = "Error: Data value invalid! ";
		return -1;
	}

	file = Openi2cDev(i2cbus, message);
	if(file < 0)
	{	
		message = ("Error : Could not open file " + std::string("dev/i2c-") + std::to_string(i2cbus));
		return -1;
	}

	if (SetSlaveAddr(file, chipAddress, message) < 0)
	{
		return -1;
	}

	//b and w mode supported
	switch(mode)
	{
		case 'b':
		{
			res = i2c_smbus_write_byte_data(file, dataAddress, value);
			break;
		}	
		case 'w':
		{
			res = i2c_smbus_write_word_data(file, dataAddress, value);
			break;
		}
		default :
		{
			res = i2c_smbus_write_byte_data(file, dataAddress, value);
			break;
		}
	}

	if(res < 0)
	{
		message = "Error : Write failed ";
		return -1;
	}

	return res;
}
int I2CAdapter::Openi2cDev(int i2cbus, std::string &message)
{
	int file;
	std::string filename = "dev/i2c-" + std::to_string(i2cbus);
	file = open(filename.c_str() ,O_RDWR);
	return file;
	//Add further error handling if required
}

int I2CAdapter::SetSlaveAddr(int file , int chipAddress ,std::string &message) // if required add force flag in this function 
{	
	std::stringstream ss;
	if(ioctl(file, I2C_SLAVE, chipAddress) < 0)
	{	
		ss << std::hex << std::setw(2) << std::setfill('0') << chipAddress ; //to return error message address in hex format
		message = "Could n qot set address to 0x" + ss.str(); 
		return -errno;
	}
	return 0;
}