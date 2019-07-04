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
    messageHandler.AddDaemonRequest("DaemonCLI", argv[1], argv[2], argv[3], argv[4], secondValue);
    std::unique_ptr<DaemonRequestT> req;
    messageHandler.TransferData(req);
    
    std::cout << "--------" << std::endl << "Response" << std::endl;
      auto union_type = req->data.type; //error
    if (union_type == PacketData::StrParamPacket) 
    {
        auto strParamPacket = req->data.AsStrParamPacket();//error
        std::string command = req->header->command; //works
        std::string parameter = req->header->parameter; //works
        std::string& message = req.get()->header->message; //works
        std::string& value1 = strParamPacket->value1; //error
        std::string& value2 = strParamPacket->value2; //error
        // bool result = module->HandleParameter(req->header->command, req->header->parameter, &strParamPacket->value1, &strParamPacket->value2, req.get()->header->message);
    std::cout << "Value: " << strParamPacket->value1 << std::endl;

    }
    // std::cout << "Value: " << req.get()->value1 << std::endl;
    std::cout << "Message: " << req.get()->header->message << std::endl;
    std::cout << "--------" << std::endl;

    return 0;
}
