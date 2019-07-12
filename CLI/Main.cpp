#include <iostream>

#include "../API_WS/MessageHandler.h"

#include <fstream>
#include "json/json.hpp"
using json = nlohmann::json;

int HandleCommandLine(const int& argc, char* argv[], std::string& secondValue)
{
    switch(argc)
    {
    case 3:
        argv[3] =  const_cast<char*>("");
        break;
    case 4:
        argv[4] = const_cast<char*>("");
        break;
    case 5:
        break;
    case 6:
        secondValue = argv[5];
        break;
    default:
        std::cout << "Not enough arguments." << std::endl;
        std::cout << "Example 1: ./DaemonCLI image_sensor get analog_gain" << std::endl;
        std::cout << "Example 2: ./DaemonCLI image_sensor set analog_gain 2" << std::endl;
        std::cout << "Example 3: ./DaemonCLI image_sensor set config_register 115 11" << std::endl;
        return 1;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    std::string secondValue = "";
    if(HandleCommandLine(argc, argv, secondValue) == 1)
    {
        return 0;
    }

    MessageHandler messageHandler;
    std::unique_ptr<DaemonRequestT> req;
    
    if(std::string(argv[1]) == "lut_conf")
    {   
        //create blob buffer
        std::ifstream lutConf(argv[4], std::ios::binary); 
        std::vector<uint8_t> lutBuffer(std::istreambuf_iterator<char>(lutConf), {}); 
        
        messageHandler.AddDaemonBlobRequest("DaemonCLI", argv[1], argv[2], argv[3], lutBuffer); //send request with buffer

        messageHandler.TransferData(req);

        std::cout << "--------" << std::endl << "Response" << std::endl;
        std::cout << "Message: " << req.get()->header->message << std::endl;
        std::cout << "--------" << std::endl;

    }
    else
    {   
        messageHandler.AddDaemonStrParamRequest("DaemonCLI", argv[1], argv[2], argv[3], argv[4], secondValue);
        messageHandler.TransferData(req);
        
        auto strParamPacket = req->data.AsStrParamPacket();

        std::cout << "--------" << std::endl << "Response" << std::endl;
        std::cout << "Value: " << strParamPacket->value1 << std::endl;
        std::cout << "Message: " << req.get()->header->message << std::endl;
        std::cout << "--------" << std::endl;
    }
      
    return 0;
}
