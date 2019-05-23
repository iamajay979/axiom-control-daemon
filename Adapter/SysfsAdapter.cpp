#include "SysfsAdapter.h"

#include "MemoryAdapterDummy.h"

#ifdef ENABLE_MOCK
using MemAdapter = MemoryAdapterDummy;
#else
using MemAdapter = MemoryAdapter;
#endif

#define GETTER_FUNC(A) std::bind(A, this, std::placeholders::_1, std::placeholders::_2)
#define SETTER_FUNC(A) std::bind(A, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)

SysfsAdapter::SysfsAdapter()
{
	RegisterAvailableMethods();
}

void SysfsAdapter::RegisterAvailableMethods()
{
	AddParameterHandler("zynq_info", GETTER_FUNC(&SysfsAdapter::GetZynqInfo), SETTER_FUNC(&SysfsAdapter::SetZynqInfo));
}

SysfsAdapter::~SysfsAdapter()
{

}

bool SysfsAdapter::GetZynqInfo(std::string& zynqInfo, std::string& message)
{	
	UNUSED(message);

	std::string ZTO,ZTR,ZTS;
	float ZT;
	std::vector<std::string> filenames;

	Glob("/sys/devices/soc0/amba/*.adc/iio*/in_temp0_offset",filenames,message);
	ReadFile(filenames[0],ZTO);
	Glob("/sys/devices/soc0/amba/*.adc/iio*/in_temp0_raw",filenames,message);
	ReadFile(filenames[1],ZTR);
	Glob("/sys/devices/soc0/amba/*.adc/iio*/in_temp0_scale",filenames,message);
	ReadFile(filenames[2],ZTS);

	ZT = ( std::stod(ZTR) + std::stod(ZTO) ) * std::stod(ZTS) / 1000 ;

	zynqInfo = std::to_string(ZT);

	return true;
}

bool SysfsAdapter::SetZynqInfo(std::string zynqInfo, std::string, std::string& message)
{
	UNUSED(zynqInfo);

	message = "Set Method Not Allowed for the given parameter ";

	return true;
}
 
void SysfsAdapter::ReadFile(std::string filename,std::string &message)
{	


	char buffer[1024]; //Is this sufficient ?
	ssize_t len;
	int fd;


	fd = open(filename.c_str(),O_RDONLY);
	if(fd < 0)
	{
		message = "Error opening file !";
		return;
	}

	len = read(fd, buffer, sizeof(buffer) -1);
	if(len < 0)
	{
		message = "Error reading file !";
		return;
	}
	buffer[len] = 0;

	message = buffer ;
	close(fd);
}

void SysfsAdapter::Glob(const std::string filename,std::vector<std::string> &filenames,std::string &message)
{
    glob_t glob_result;
    memset(&glob_result, 0, sizeof(glob_result));

	int return_value = glob(filename.c_str(), GLOB_TILDE, NULL, &glob_result);

    if(return_value != 0) 
    {
    	globfree(&glob_result);
    	message = "Error : glob() failed !!";
    	return;
    }

    for(size_t i = 0; i < glob_result.gl_pathc; ++i) 
    {
        filenames.push_back(std::string(glob_result.gl_pathv[i]));
    }

    globfree(&glob_result);

}