#include "LutConfModule.h"

#include "../Adapters/MemoryAdapterDummy.h"

#ifdef ENABLE_MOCK
using MemAdapter = MemoryAdapterDummy;
#else
using MemAdapter = MemoryAdapter;
#endif

#define GETTER_FUNC(A) std::bind(A, this, std::placeholders::_1, std::placeholders::_2)
#define SETTER_FUNC(A) std::bind(A, this, std::placeholders::_1, std::placeholders::_2)

LutConfModule::LutConfModule() :
    address(0x80300000),
    memorySize(0x000C0000),
    _memoryAdapter(std::make_shared<MemAdapter>())
{
    // Map the regions at start, to prevent repeating calls of mmap()
    _memoryAdapter->MemoryMap(address, memorySize);

    RegisterAvailableMethods();
}

void LutConfModule::RegisterAvailableMethods()
{
    AddBlobParameterHandler("gamma_correction_lut", GETTER_FUNC(&LutConfModule::GetLut), SETTER_FUNC(&LutConfModule::SetLut));
}

LutConfModule::~LutConfModule()
{
    _memoryAdapter->MemoryUnmap(address, memorySize);
}
void LutConfModule::ParseValues (std::vector<uint8_t>& lutBuffer, std::vector<int32_t>& values, std::string& message)
{	
	//Processing Depends on the format of data in the lut_conf file (Considering all values in seperate rows for testing)
	std::string temp = "";
	int count = 0;
	for(unsigned int i = 0; i < lutBuffer.size(); i++)
	{
		if(lutBuffer[i] == '\n')
		{
			++count;

			try
			{
				values[count] = stoi(temp);
			}
			catch(const std::invalid_argument& ia)
			{
				message = "Error Getting LUT Values . Check File format" ;
				return ;
			}
			temp = "";
		}
		else
		{
			temp += lutBuffer[i];
		}
	}

	try
	{
		values[count] = stoi(temp);
	}
	catch(const std::invalid_argument& ia)
	{
		message = "Error Getting LUT Values . Check File format" ;
		return ;
	}
}
bool LutConfModule::SetLut(std::vector<uint8_t>& lutBuffer, std::string& message)
{
	std::vector<int32_t> values(4096*4);
	ParseValues(lutBuffer, values, message);

	for(unsigned int i = 0; i < values.size(); i++)
	{
		// std::cout<<values[i]<<std::endl;
		_memoryAdapter->WriteWord(i, values[i]);
	}
	return true;
}

bool LutConfModule::GetLut(std::vector<uint8_t>& lutBuffer, std::string& message)
{
	message = "Get parameter not allowed for this parameter";

	return true;
}