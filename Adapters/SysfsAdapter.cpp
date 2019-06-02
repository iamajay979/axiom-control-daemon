#include "SysfsAdapter.h"

SysfsAdapter::SysfsAdapter()
{
}

void SysfsAdapter::ReadByte(uint8_t data)
{
    UNUSED(data);
}

void SysfsAdapter::WriteByte(uint8_t data)
{
    UNUSED(data);
}

void SysfsAdapter::ReadBlock(uint8_t *data, unsigned int length)
{
    UNUSED(data);
    UNUSED(length);
}

void SysfsAdapter::WriteBlock(uint8_t *data, unsigned int length)
{
    UNUSED(data);
    UNUSED(length);
}

void SysfsAdapter::Execute() {}

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