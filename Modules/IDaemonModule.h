#ifndef IDAEMONMODULE_H
#define IDAEMONMODULE_H

#include <functional>
#include <vector>
#include <string>
#include <unordered_map>
#include <iostream> //remove this 

#include "../Log/Logger.h"

class IDaemonModule
{
    // string& value, string& message, returns: bool - success or fail
    using GetterFunc = std::function<bool(std::string&, std::string&)>;
    // string value1, string value2, string& message, returns: bool - success or fail
    using SetterFunc = std::function<bool(std::string, std::string, std::string&)>;

    using BlobSetterFunc = std::function<bool(std::vector<uint8_t>&, std::string&)>;
    using BlobGetterFunc = std::function<bool(std::vector<uint8_t>&, std::string&)>;
    // chipAddress, DataAddress, value, mode,message
    using I2cGetterFunc = std::function<bool(std::string, std::string, std::string&, std::string, std::string&)>;
    using I2cSetterFunc = std::function<bool(std::string, std::string, std::string, std::string, std::string&)>;


    struct ParameterHandler
    {
        GetterFunc Getter;
        SetterFunc Setter;
    };

    struct BlobParameterHandler
    {
        BlobGetterFunc Getter;
        BlobSetterFunc Setter;
    };

    struct I2cParameterHandler
    {
        I2cGetterFunc Getter;
        I2cSetterFunc Setter;
    };

    std::unordered_map<std::string, ParameterHandler> parameterHandlers;
    std::unordered_map<std::string, BlobParameterHandler> blobParameterHandlers;
    std::unordered_map<std::string, I2cParameterHandler> i2cParameterHandlers;

    // TODO: Don't remove commented and related parts yet, maybe we need individual methods later, to trigger special functionality, which ar not related to parameters directly
    //typedef std::function<bool(std::string&)> CallbackFunc;

    //std::unordered_map<std::string, CallbackFunc> _registeredMethods;
    //std::unordered_map<std::string, CallbackFunc>::const_iterator it;

    //void RegisterMethods(std::string name, CallbackFunc func)
    //{
    //    _registeredMethods.emplace(std::make_pair(name, func));
    //}

    //std::vector<std::string> GetRegisteredMethodNames()
    //{
    //    std::vector<std::string> keys;
    //
    //    for(auto kv : _registeredMethods)
    //    {
    //        keys.push_back(kv.first);
    //    }
    //
    //    return keys;
    //}

    //bool ProcessMethod(std::string methodName, std::string value)
    //{
    //    it = _registeredMethods.find(methodName);
    //
    //    if(it != _registeredMethods.end())
    //    {
    //        return it->second(value);
    //    }
    //}


    //std::vector<std::string> GetAvailableMethods()
    //{
    //    return GetRegisteredMethodNames();
    //}

protected:
    virtual void RegisterAvailableMethods() = 0;

    void AddParameterHandler(std::string name, GetterFunc getter, SetterFunc setter)
    {
        parameterHandlers.insert(std::make_pair(name, ParameterHandler{getter, setter}));
    }

    void AddBlobParameterHandler(std::string name, BlobGetterFunc getter, BlobSetterFunc setter)
    {
        blobParameterHandlers.insert(std::make_pair(name, BlobParameterHandler{getter, setter}));
    }

    void AddI2cParameterHandler(std::string name, I2cGetterFunc getter, I2cSetterFunc setter)
    {
        i2cParameterHandlers.insert(std::make_pair(name, I2cParameterHandler{getter, setter}));
    }

public:
    virtual ~IDaemonModule() = default;

    //virtual std::vector<std::string>GetAvailableMethods() = 0;

    //TODO : Make function to do common tasks of HandleBlobParameter and HandleParameter
    bool HandleBlobParameter(std::string command, std::string parameterName, std::vector<uint8_t>& parameterValue, std::string& message)
    {   

        // TODO : Check if two parameter values are required
        std::string originalParameterName = parameterName;
        std::unordered_map<std::string, BlobParameterHandler>::const_iterator got = blobParameterHandlers.find(parameterName);

        if ( got == blobParameterHandlers.end() )
        {
            DAEMON_LOG_ERROR("Handler not found");
            message = "Handler not found: " + parameterName;
            return false;
        }
        else
        {
            DAEMON_LOG_INFO("Handler found");

            auto handler = got->second;
            //auto method = (command == "set") ? handler.Setter : handler.Getter;
            bool result = false;
            if(command == "set")
            {   
                result = handler.Setter(parameterValue, message); 
                // std::cout<<result<<std::endl;
            }
            else if(command == "get")
            {
                result = handler.Getter(parameterValue, message); 
            }

            return result;
        }
    }
    bool HandleI2cParameter(std::string command, std::string parameterName, std::string& parameterValue1, std::string& parameterValue2, std::string& parameterValue3, std::string& parameterValue4, std::string& message)
    {
        std::string originalParameterName = parameterName;
        std::unordered_map<std::string, I2cParameterHandler>::const_iterator got = i2cParameterHandlers.find(parameterName);

        if ( got == i2cParameterHandlers.end() )
        {
            DAEMON_LOG_ERROR("Handler not found");
            message = "Handler not found: " + parameterName;
            return false;
        }
        else
        {
            DAEMON_LOG_INFO("Handler found");

            auto handler = got->second;
            //auto method = (command == "set") ? handler.Setter : handler.Getter;
            bool result = false;
            if(command == "set")
            {
                result = handler.Setter(parameterValue1, parameterValue2, parameterValue3, parameterValue4, message);
            }
            else if(command == "get")
            {   
                std::cout<<"=========================="<<std::endl;
                std::cout<<parameterValue1<<" "<<parameterValue2<<" "<<parameterValue3<<" "<<parameterValue4<<" "<<message<<std::endl;
                result = handler.Getter(parameterValue1, parameterValue2, parameterValue3, parameterValue4, message);
            }

            return result;
        }
    }
    bool HandleParameter(std::string command, std::string parameterName, std::string& parameterValue1, std::string& parameterValue2, std::string& message)
    {
        std::string originalParameterName = parameterName;
        std::unordered_map<std::string, ParameterHandler>::const_iterator got = parameterHandlers.find(parameterName);
        
        if ( got == parameterHandlers.end() )
        {
            DAEMON_LOG_ERROR("Handler not found");
            message = "Handler not found: " + parameterName;
            return false;
        }
        else
        {
            DAEMON_LOG_INFO("Handler found");

            auto handler = got->second;
            //auto method = (command == "set") ? handler.Setter : handler.Getter;
            bool result = false;
            if(command == "set")
            {
                result = handler.Setter(parameterValue1, parameterValue2, message);
            }
            else if(command == "get")
            {
                result = handler.Getter(parameterValue1, message);
            }

            return result;
        }
    }

};

#endif //IDAEMONMODULE_H
