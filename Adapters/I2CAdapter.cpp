#include "I2CAdapter.h"

#define BUFFERSIZE 10
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

int I2CAdapter::ReadRegister(int busFd, int dataAddress, int size, std::string &message)
{
	char reg_buf[1];
	char buff[BUFFERSIZE];
	std::stringstream ss;
	int res;

	if(dataAddress != -1) //Data address is provided ,hence set pointer to it, else read default add.
	{
		reg_buf[0] = dataAddress;

		res = SetAddressPointer(busFd, reg_buf, 1);
		if(res != 1)
		{
			message = "Error : Failed to set address pointer";
			return -1;
		}
	}

	res = read(busFd, buff, size);
	if(res != size)
	{	
		ss << "0x" << std::hex << dataAddress;
		message = "Error : Failed to read " + ss.str();
		return -1;
	}

	ss<<"0x";  //Convert char buffer to hex string
	for (int i = size - 1;i >= 0 ; i--)
	{	
		ss<<std::hex<<static_cast<int>(buff[i]);
	}
	
	res = std::stol(ss.str(), nullptr, 0);

	return res;
}

int I2CAdapter::WriteRegister(int busFd, int dataAddress, int value, int size, std::string &message)
{	
	char buff[BUFFERSIZE];
	std::stringstream ss;
	int res;

	if(size == 2)
	{
		buff[0] = dataAddress;
		buff[1] = value;
	}
	if(size == 3)
	{
		buff[0] = dataAddress;
		buff[1] = (value >> 0) & 0xFF;
		buff[2] = (value >> 8) & 0xFF;
	}

	res = write(busFd, buff, size);
	if(res != size)
	{
		ss << "0x" << std::hex << dataAddress;
		message = "Error : Failed to write at " + ss.str();
		return -1;
	}

	return res;
}

int I2CAdapter::SetAddressPointer(int busFd, char *buff, int size)
{
	int res;
	res = write(busFd, buff, size);

	return res;
}

int I2CAdapter::StrToInt(std::string hexStr)
{   
	int val;
	try
	{
		val = std::stoi(hexStr, nullptr, 0);
	}
	catch(const std::invalid_argument& ia)
	{
		return -2;
	}

	return val;
}

int I2CAdapter::I2cGet(std::string i2cbusArg, std::string chipAddressArg,std::string dataAddressArg, char mode ,std::string &message)
{	
	//Assumption is that the i2cbus is a valid integer, if names of i2cbus has to incorporated then a proper lookup function has to created similar to system func "i2cget"
	//Allowed modes : 'b', 'w', 'c'
	int i2cbus, chipAddress, dataAddress;
	int file, res, size; 

	i2cbus = StrToInt(i2cbusArg);
	if(i2cbus < 0)
	{
		message = "Error : Invalid argument";
		return -1;
	}
	if (i2cbus > 0xFFFFF)
	{
		message = "Error : I2C bus out of range ";
		return -1;
	}

	chipAddress = StrToInt(chipAddressArg);
	if(chipAddress < 0)
	{
		message = "Error : Invalid argument";
		return -1;	
	}
	if(chipAddress < 0x03 || chipAddress > 0x77) //Replace them with const var ?
	{
		message = " Error : Chip Address out of range ";
		return -1;
	}

	if (dataAddressArg != "-1")
	{
		dataAddress = StrToInt(dataAddressArg);
		if(dataAddress < 0)
		{
			message = "Error : Invalid argument";
			return -1;
		}
		if(dataAddress > 0xff)//handle negative except -1
		{	
			message = "Error : Data Address out of range";
			return -1;
		}
	}
	else
	{
		dataAddress = -1;
	}

	file = Openi2cDev(i2cbus, message);

	if(file < 0)
	{	
		message = ("Error : Could not open file " + std::string("/dev/i2c-") + std::to_string(i2cbus));
		return -1;
	}
	if (SetSlaveAddr(file, chipAddress, message) < 0)
	{
		return -1;
	}

	switch(mode)
	{
		case 'b' : //read byte
		{ 	
			size = 1;
			break;
		}
		case 'w' : //read word
		{	
			size = 2;
			break;
		}
		default :
		{
			message = "Only byte(b) and word(w) operations are Allowed";
			return -1;		
		}
	}

	res = ReadRegister(file, dataAddress, size, message);
	if (res < 0)
	{
		message = "Error : Read Failed ";
		return -1;
	}

	return res;
}

int I2CAdapter::I2cSet(std::string i2cbusArg, std::string chipAddressArg,std::string dataAddressArg, std::string valueArg, char mode , std::string &message)
{
	int i2cbus, chipAddress, dataAddress ,value;
	int file, res, size;

	i2cbus = StrToInt(i2cbusArg);
	if(i2cbus < 0)
	{
		message = "Error : Invalid argument";
		return -1;
	}
	if (i2cbus > 0xFFFFF)
	{
		message = "Error : I2C bus out of range ";
		return -1;
	}

	chipAddress = StrToInt(chipAddressArg);
	if(chipAddress < 0)
	{
		message = "Error : Invalid argument";
		return -1;	
	}
	if(chipAddress < 0x03 || chipAddress > 0x77) //Replace them with const var ?
	{
		message = " Error : Chip Address out of range ";
		return -1;
	}

	dataAddress = StrToInt(dataAddressArg);
	if(dataAddress < 0)
	{
		message = "Error : Invalid argument";
		return -1;
	}
	if(dataAddress > 0xff)//handle negative except -1
	{	
		message = "Error : Data Address out of range";
		return -1;
	}

	value = StrToInt(valueArg);
	if(value < 0)
	{
		message = "Error : Invalid argument";
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
		message = ("Error : Could not open file " + std::string("/dev/i2c-") + std::to_string(i2cbus));
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
			size = 2;
			break;
		}	
		case 'w':
		{	
			size = 3;
			break;
		}
		default :
		{
			message = "Only byte(b) and word(w) operations are Allowed";
			return -1;
		}
	}

	res = WriteRegister(file, dataAddress, value, size, message);
	if(res != size)
	{
		message = "Error : Write failed ";
		return -1;
	}

	return res;
}
int I2CAdapter::Openi2cDev(int i2cbus, std::string &message)
{
	int file;
	std::string filename = "/dev/i2c-" + std::to_string(i2cbus);
	file = open(filename.c_str(), O_RDWR);
	return file;
	//Add further error handling if required
}

int I2CAdapter::SetSlaveAddr(int file, int chipAddress, std::string &message) // if required add force flag in this function 
{	
	std::stringstream ss;
	if(ioctl(file, I2C_SLAVE, chipAddress) < 0)
	{	
		ss << std::hex << std::setw(2) << std::setfill('0') << chipAddress ; //to return error message address in hex format
		message = "Error : Could not set address to 0x" + ss.str(); 
		return - errno;
	}
	return 0;
}