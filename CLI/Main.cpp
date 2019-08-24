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

// Used different method as in different types of requests the positional parameters have different meaning, also incase if the command given is wrong then examples 
// of only that module will be given
int HandleCommandLineI2C(const int& argc, char* argv[], std::string& secondValue, std::string& thirdValue, std::string& fourthValue)
{
    switch(argc)
    {
    case 5:
        if(std::string(argv[2]) == "get")
        {
            secondValue = "-1"; //dataAddress
            fourthValue = "b";  //mode (byte is default)
        }
        else
        {
            std::cout << "Invalid Command" << std::endl;
            std::cout << "Example 1: ./DaemonCLI i2c get busAddr chipAddr" << std::endl;
            return 1;
        }
        break;
    case 6:
        if(std::string(argv[2]) == "get")
        {
            if(std::string(argv[5]) == "b" || std::string(argv[5]) == "w") // Command of type : "DaemonCLI i2c get i2c# chipAddr mode" 
            {
                secondValue = "-1"; //dataAddress
                fourthValue = argv[5]; //mode
            }
            else // Command of type : "DaemonCLI i2c get i2c# chipAddr dataAddr"
            {
                secondValue = argv[5]; //dataAddress
                fourthValue = "b"; //mode
            }
        }
        else
        {
            std::cout << "Invalid Command" << std::endl;
            std::cout << "Example 1: ./DaemonCLI i2c get busAddr chipAddr mode" << std::endl;
            std::cout << "Example 2: ./DaemonCLI i2c get busAddr chipAddr dataAddress" << std::endl;
        }
        break;
    case 7:
        if(std::string(argv[2]) == "get")
        {
            secondValue = argv[5]; //dataAddress
            fourthValue = argv[6]; //mode 
        }
        else if(std::string(argv[2]) == "set")
        {
            secondValue = argv[5]; //dataAddress
            thirdValue = argv[6]; //value
            fourthValue = "b"; //mode
        }
        else
        {
            std::cout << "Invalid Command" << std::endl;
            std::cout << "Example 1: ./DaemonCLI i2c get busAddr chipAddr dataAddress mode" << std::endl;
            std::cout << "Example 2: ./DaemonCLI i2c set busAddr chipAddr dataAddress value" << std::endl;
        }
        break;
    case 8:
        if(std::string(argv[2]) == "set")
        {
            secondValue = argv[5];
            thirdValue = argv[6];
            fourthValue = argv[7];
        }
        else
        {
            std::cout << "Invalid Command" << std::endl;
            std::cout << "Example 1: ./DaemonCLI i2c set busAddr chipAddr dataAddress value mode" << std::endl;
        }
        break;
    default:
        std::cout << "Not enough arguments." << std::endl;
        std::cout << "Example 1: ./DaemonCLI i2c get busAddr chipAddr" << std::endl;
        std::cout << "Example 2: ./DaemonCLI i2c get busAddr chipAddr mode " << std::endl;
        std::cout << "Example 3: ./DaemonCLI i2c get busAddr chipAddr dataAddr" << std::endl;
        std::cout << "Example 4: ./DaemonCLI i2c set busAddr chipAddr dataAddr value" << std::endl;
        std::cout << "Example 5: ./DaemonCLI i2c set busAddr chipAddr dataAddr value mode" << std::endl;

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

    // if(HandleCommandLine(argc, argv, secondValue, thirdValue, fourthValue) == 1)
    // {
    //     return 0;
    // }

    MessageHandler messageHandler;
    std::unique_ptr<DaemonRequestT> req;

    if(std::string(argv[1]) == "i2c")
    {   
        //Allowed commands for now

        // 1) DaemonCLI i2c get i2c# chipAddr 
        // 2) DaemonCLI i2c get i2c# chipAddr mode 
        // 2) DaemonCLI i2c get i2c# chipAddr dataAddr 
        // 3) DaemonCLI i2c get i2c# chipAddr dataAddr mode 
        // 4) DaemonCLI i2c set i2c# chipAddr dataAddr value 
        // 5) DaemonCLI i2c set i2c# chipAddr dataAddr value mode

        if(HandleCommandLineI2C(argc, argv, secondValue, thirdValue, fourthValue) == 1)
        {
            return 0;
        }
                                       //./DaemonCLI       i2c    get      i2c0     chip     data         value       mode
        messageHandler.AddDaemonI2cRequest("DaemonCLI", argv[1], argv[2], argv[3], argv[4], secondValue, thirdValue, fourthValue);
        messageHandler.TransferData(req);

        auto i2cPacket = req->data.AsI2cPacket();

        std::cout << "--------" << std::endl << "Response" << std::endl;
        std::cout << "Value: " << i2cPacket->value3 << std::endl;
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
        if(HandleCommandLine(argc, argv, secondValue) == 1)
        {
            return 0;
        }

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
