#include <iostream>

#include "../API_WS/MessageHandler.h"

#include <fstream>
#include "json/json.hpp"
using json = nlohmann::json;

int HandleCommandLine(const int& argc, char* argv[], std::string& secondValue, std::string& thirdValue, std::string& fourthValue)
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
    case 7:
        secondValue = argv[5];
        thirdValue = argv[6];
        break;
    case 8:
        fourthValue = argv[7];
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
    // TODO : Rework CLI code in a clean structure
    std::string secondValue = "";
    std::string thirdValue = "";
    std::string fourthValue = "";

    if(HandleCommandLine(argc, argv, secondValue, thirdValue, fourthValue) == 1)
    {
        return 0;
    }

    MessageHandler messageHandler;
    std::unique_ptr<DaemonRequestT> req;

    if(std::string(argv[1]) == "i2c")
    {   
        //Allowed commands
        // 1) DaemonCLI i2c get i2c* chipAddr 
        // 2) DaemonCLI i2c get i2c* chipAddr dataAddr
        // 3) DaemonCLI i2c get i2c* chipAddr dataAddr mode
        // 4) DaemonCLI i2c set i2c* chipAddr dataAddr value
        // 5) DaemonCLI i2c set i2c* chipAddr dataAddr value mode
        if(argc < 5)
        {
            std::cout << "Not enough arguments" << std::endl;
            return 0;
        }
        if(std::string(argv[2]) == "set" && argc < 7)
        {
            std::cout << "Not enough arguments" << std::endl;
            return 0;
        }
        if(argc == 5)
        {
            secondValue = "-1"; //dataAddress
            fourthValue = "b";  //mode
        }
        if(argc == 6)
        {
            fourthValue = "b"; //mode
        }
        if(argc == 7 && std::string(argv[2]) == "set")
        {
            fourthValue = "b"; //mode
        }
                                       //./DaemonCLI   i2c0     get      i2c0     chip     data         value       mode
        messageHandler.AddDaemonI2cRequest("DaemonCLI", argv[1], argv[2], argv[3], argv[4], secondValue, thirdValue, fourthValue);
        messageHandler.TransferData(req);

        auto i2cPacket = req->data.AsI2cPacket();

        std::cout << "--------" << std::endl << "Response" << std::endl;
        std::cout << "Value: " << i2cPacket->value1 << std::endl;
        std::cout << "Message: " << req.get()->header->message << std::endl;
        std::cout << "--------" << std::endl;

    }
    
    else if(std::string(argv[1]) == "lut_conf")
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
