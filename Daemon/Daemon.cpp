#include "Daemon.h"

#include <chrono>
#include <iomanip>
#include <sstream>
#include <thread>

#include "../Log/DefaultLogger.h"

#define BUFFER_MAX 163840

std::string GetCurrentTimestamp()
{
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
    return ss.str();
}

Daemon::Daemon() :
    _socketPath("/tmp/axiom_daemon.uds"),
    _processID(std::to_string(getpid())),
    _running(true),
    _socketDesc(0)
{
    DAEMON_LOG_INFO("Daemon initialization");

    // TODO (BAndiT1983): Rework to map, see image_sensor example below
    //_memoryAdapter = new MemoryAdapter();
    //_i2cAdapter = new I2CAdapter();

    _modules["image_sensor"] = std::make_shared<CMV12000Module>();
    _modules["temperatures"] = std::make_shared<EnvironmentModule>();
    _modules["voltages"] = std::make_shared<EnvironmentModule>();
    _modules["i2c_test"] = std::make_shared<I2CTestModule>();
    _modules["lut_conf"] = std::make_shared<LutConfModule>();
    _modules["i2c"] = std::make_shared<I2CHelperModule>();
    // TODO (BAndiT1983): Add real reading of revision/version
    //std::string/int?? revision = ReadRevision();
    //std::string revision = "29";

    // TODO (BAndiT1983): Adjust paths to real ones, this ones are just for testing
    // TODO (BAndiT1983): Add fallback to older revision version if for current one no file is available
    //_memoryAdapter->ReadDescriptions("../Descriptions/Memory_rev" + revision + ".json");
    //_i2cAdapter->ReadDescriptions("../Descriptions/I2C_rev" + revision + ".json");

    std::ifstream ifs("description.json");
    availableParameters = json::parse(ifs);
}

Daemon::~Daemon()
{
    //delete _memoryAdapter;
    //delete _i2cAdapter;
}

void Daemon::Setup()
{
    DAEMON_LOG_INFO("Socket initialization");
    SetupSocket();
}

void Daemon::Start()
{
    Process();
}

void Daemon::Process()
{
    unsigned int addrlen = sizeof (_socketDesc);
    
    //std::thread t(&Daemon::ProcessClient, this, std::placeholders::_1);

    while(_running)
    {
        std::cout << "Waiting for client to connect" << std::endl;
        // TODO (BAndiT1983): Add handling of multiple clients, e.g. each processing in new thread, also check thread-safety of STL vector, to place requests in a queue
        int new_socket = accept (_socketDesc, reinterpret_cast<struct sockaddr*>(&new_socket), &addrlen);
        std::cout << "Socket (loop): " << new_socket << std::endl;

        if(new_socket)
        {
            std::thread t(&Daemon::ProcessClient, this, new_socket);
            t.join();
        }

        //close(new_socket);
    }
}

void Daemon::ProcessClient(int socket)
{
    uint8_t* receivedBuffer = new uint8_t[BUFFER_MAX];
    std::cout << "NEW CLIENT" << std::endl;
    std::cout << "Socket: " << socket << std::endl;

    while(1)
    {
        if(socket < 0)
        {
            printf("ACCEPT ERROR = %s\n", strerror(errno));
            return;
        }

        // Wait for packets to arrive
        ssize_t size = RetrieveIncomingData(socket, receivedBuffer, BUFFER_MAX);
        if(size <= 0)
        {
            return;
        }

        std::cout << "Received buffer size: " << receivedBuffer << std::endl;
        ProcessReceivedData(receivedBuffer);
        ssize_t error = send(socket, _builder.GetBufferPointer(), _builder.GetSize(), 0);
        if(error < 0)
        {
            std::cout << "Error while sending response." << std::endl;
            printf("SEND ERROR = %s\n", strerror(errno));
        }
    _builder.Clear();
    }
}

void Daemon::ProcessReceivedData(uint8_t* receivedBuffer)
{
    auto req= UnPackDaemonRequest(receivedBuffer);//req is an object that is made from the buffer via UnPackDaemonRequest

    std::string moduleName = req.get()->header->module_;
    if(moduleName == "general")
    {
        ProcessGeneralRequest(req);

        _builder.Finish(CreateDaemonRequest(_builder, req.get()));

        return;
    }

    _module_iterator = _modules.find(moduleName);
    std::string message = "";
    if (_module_iterator == _modules.end())
    {
        message = "Received: Unknown module";
        DAEMON_LOG_INFO("Received: Unknown module");
        req.get()->header->message = message;
        _builder.Finish(CreateDaemonRequest(_builder, req.get()));
        return;
    }

    auto module = _module_iterator->second;
    auto union_type = req->data.type;

    if (union_type == PacketData::StrParamPacket) 
    {   
        auto strParamPacket = req->data.AsStrParamPacket();
        bool result = module->HandleParameter(req->header->command, req->header->parameter, strParamPacket->value1, strParamPacket->value2, req.get()->header->message);
    }
    else if (union_type == PacketData::BlobPacket)
    {   
        auto blobPacket = req->data.AsBlobPacket();
        bool result = module->HandleBlobParameter(req->header->command, req->header->parameter, blobPacket->value, req->header->message);
    }
    else
    {   // I2cPacket
        auto i2cPacket = req->data.AsI2cPacket();// get                  i2c0                  chip               data               value              mode
        bool result = module->HandleI2cParameter(req->header->command, req->header->parameter, i2cPacket->value1, i2cPacket->value2, i2cPacket->value3, i2cPacket->value4, req.get()->header->message);
    }
   
    // TODO (BAndiT1983):Check if assignments are really required, or if it's suitable of just passing reference to req attirbutes
    bool result = true;
    req.get()->header->status = (result == true) ? "STATUS_SUCCESS" : "STATUS_FAIL";
    req.get()->header->timestamp = GetCurrentTimestamp();
    _builder.Finish(CreateDaemonRequest(_builder, req.get()));

}

bool Daemon::ProcessGeneralRequest(std::unique_ptr<DaemonRequestT> &req)
{   
    bool result = false;
    req.get()->header->status = "fail";
    auto union_type = req->data.type; 

    if (union_type == PacketData::StrParamPacket) 
    {
        auto strParamPacket = req->data.AsStrParamPacket();
    
        if(req.get()->header->command == "get" && req.get()->header->parameter == "available_parameters")
        {
            ProcessAvailableParameters(std::bind(&Daemon::RetrieveCurrentParameterValues, this, std::placeholders::_1, std::placeholders::_2));

            strParamPacket->value1 = availableParameters.dump();
            req.get()->header->message = "";
            req.get()->header->status = "success";
            req.get()->header->timestamp = GetCurrentTimestamp();
        }
        else if(req.get()->header->command == "reset")
        {
            ProcessAvailableParameters(std::bind(&Daemon::ResetParameterValues, this, std::placeholders::_1, std::placeholders::_2));

            strParamPacket->value1 = availableParameters.dump();
            req.get()->header->message = "";
            req.get()->header->status = "success";
            req.get()->header->timestamp = GetCurrentTimestamp();
        }
    }
    else if (union_type == PacketData::BlobPacket)
    {
        // handle generalblob request if any in future
        // std::cout<<"Handling for blob packet not found"<<std::endl;
    }
    else
    {
        // handle for general I2cRequest if any in future;
    }

    return result;
}

void Daemon::SetupSocket()
{
    std::string errorMessage;

    unlink(_socketPath.c_str()); // Unlink socket to ensure that new connection will not be refused

    _socketDesc = socket(AF_LOCAL, SOCK_SEQPACKET, 0);
    if (_socketDesc < 0)
    {
        errorMessage = "Socket error: " + std::string(strerror(errno));
    }

    _name.sun_family = AF_LOCAL;
    strcpy(_name.sun_path, _socketPath.c_str());

    int result = bind(_socketDesc, reinterpret_cast<struct sockaddr*>(&_name), sizeof(_name));
    if (result < 0)
    {
        _statusMessage = "Bind failed: " + std::string(strerror(errno));
        DAEMON_LOG_ERROR(_statusMessage);
        exit(1);
    }

    listen(_socketDesc, 5);
}

ssize_t Daemon::RetrieveIncomingData(int socket, uint8_t* receivedBuffer, unsigned int bufferSize)
{
    ssize_t size = recv(socket, receivedBuffer, bufferSize, 0);
    if(size < 0)
    {
        printf("Received data size: %ld\n", size);
        printf("RECV ERROR = %s\n", strerror(errno));
    }

    return size;
}

void Daemon::ProcessAvailableParameters(std::function<void (IDaemonModule*, json&)> processingFunc)
{
    for(json::iterator it = availableParameters.begin(); it != availableParameters.end(); ++it)
    {
        _module_iterator = _modules.find(it.value()["module"]);
        if (_module_iterator == _modules.end())
        {
            std::cout << "Unknown module: " << it.value()["moduleName"];
            continue;
        }

        auto module = _module_iterator->second;

        processingFunc(module.get(), it.value());
    }
}

void Daemon::RetrieveCurrentParameterValues(IDaemonModule* module, json& description)
{
    std::string currentValue;
    std::string temp1;
    std::string temp2;
    bool result = module->HandleParameter("get", description["parameter"], currentValue, temp1, temp2);

    if(result)
    {
        description["currentValue"] = currentValue;
    }
}

void Daemon::ResetParameterValues(IDaemonModule *module, json& description)
{
    std::string currentValue = description["defaultValue"];
    std::string temp1;
    std::string temp2;
    module->HandleParameter("set", description["parameter"], currentValue, temp1, temp2);
}
