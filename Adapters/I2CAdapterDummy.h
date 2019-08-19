#ifndef MEMORYADAPTERDUMMY_H
#define MEMORYADAPTERDUMMY_H

#include "I2CAdapter.h"

class I2CAdapterDummy : public I2CAdapter
{
	int Openi2cDev(int i2cbus, std::string &message)
	{  
        UNUSED(i2cbus);
        UNUSED(message);

		return 1;
	}
    int SetSlaveAddr(int file, int chipAddress, std::string &message)
    {   
        UNUSED(chipAddress);
        UNUSED(message);

    	return 1;
    }
    int SetAddressPointer(int busFd, char *buff, int size)
    {
        UNUSED(busFd);
        UNUSED(buff);
        UNUSED(size);

    	return 1;
    }
    int WriteRegister(int busFd, int dataAddress, int value, int size, std::string &message)
    {
        UNUSED(busFd);
        UNUSED(dataAddress);
        UNUSED(value);
        UNUSED(message);

    	return size;
    }
    int ReadRegister(int busFd, int dataAddress, int size, std::string &message)
    {
        UNUSED(busFd);
        UNUSED(dataAddress);
        UNUSED(size);
        UNUSED(message);

    	return 1;
    }
};

#endif
