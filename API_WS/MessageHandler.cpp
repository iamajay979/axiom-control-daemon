#include "MessageHandler.h"

//#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>

#include <json/json.hpp>

#define BUFFER_MAX 163840

using json = nlohmann::json;

namespace ns
{
    struct JSONSetting
    {
        std::string sender;
        std::string module;
        std::string command;
        std::string parameter;
        std::string value1;
        std::string value2;
        std::string status;
        std::string message;
        std::string timestamp;
    };

    void to_json(json& j, const JSONSetting& setting)
    {
        j = json{{"sender", setting.sender},
                 {"module", setting.module},
                 {"command", setting.command},
                 {"parameter", setting.parameter},
                 {"value1", setting.value1},
                 {"value2", setting.value2},
                 {"status", setting.status},
                 {"message", setting.message},
                 {"timestamp", setting.timestamp}};
    }

    void from_json(const json& j, JSONSetting& s)
    {
        s.sender = j.at("sender").get<std::string>();
        s.module = j.at("module").get<std::string>();
        s.command = j.at("command").get<std::string>();
        s.parameter = j.at("parameter").get<std::string>();
        s.value1 = j.at("value1").get<std::string>();
        s.value2 = j.at("value2").get<std::string>();
        s.status = j.at("status").get<std::string>();
        s.message = j.at("message").get<std::string>();
        s.timestamp = j.at("timestamp").get<std::string>();
    }
};

MessageHandler::MessageHandler() : 
    socketPath("/tmp/axiom_daemon.uds"),
    _sockaddrLength(sizeof(struct sockaddr_un)),
    _response(""),
    _builder(new flatbuffers::FlatBufferBuilder())
{
    SetupSocket();
}

MessageHandler::~MessageHandler()
{
    delete _builder;
}

bool MessageHandler::ProcessMessage(std::string message, std::string& response)
{
    ns::JSONSetting setting;
    try
    {
        setting = json::parse(message);
    }
    catch(std::exception&)
    {
        response = "Invalid format";
        return false;
    }

    //change these to according to  new packet

    // AddDaemonStrParamRequest(setting.sender, setting.module, setting.command, setting.parameter, setting.value1, setting.value2);
    // std::unique_ptr<DaemonRequestT> req;
    // TransferData(req);

    //Change here for new schema
    
    // setting.value1 = req.get()->value1;
    // setting.value2 = req.get()->value2;
    // setting.message = req.get()->message;
    // setting.status = req.get()->status;
    //setting.timestamp = req.get()->timestamp;

    json j = setting;
    response = j.dump();

    return true;
}

void MessageHandler::Execute()
{
    // TODO: Implement packet to trigger applying/retrieving of settings sent to daemon
}

void MessageHandler::TransferData(std::unique_ptr<DaemonRequestT>& req)
{
    std::cout << "TransferData() started" << std::endl;
    //auto setList = _builder->CreateVector(_settings);
    _builder->Finish(_settings[0]);
    
    //send(clientSocket, _builder->GetBufferPointer(), _builder->GetSize(), 0);
    int response = sendto(clientSocket, _builder->GetBufferPointer(), _builder->GetSize(), 0, reinterpret_cast<struct sockaddr*>(&address), _sockaddrLength);
    if(response == -1)
    {
        std::cout<<"Error No : "<<errno<<std::endl;
        return ;
    }

    ssize_t i = recvfrom(clientSocket, &_response, BUFFER_MAX, 0, reinterpret_cast<struct sockaddr*>(&address), &_sockaddrLength);
    if(i < 0)
    {
        std::cout << "RECEIVE ERROR: " << strerror(errno) << std::endl;
        close(clientSocket);
        exit(1);
        //std::cout << "Response received" << std::enerrnodl;
    }

    req = UnPackDaemonRequest(_response);//DaemonRequest::UnPack(req, receivedBuffer);
    std::cout << "RESPONSE MESSAGE: " << req.get()->header->status << std::endl;
    std::string message = "Data size: " + std::to_string(_builder->GetSize());
    std::cout << message.c_str() << std::endl;
    
    // Clear settings after sending
    _settings.clear();
    _builder->Clear();

    std::cout << "TransferData() completed" << std::endl;

    std::cout << "Response (message): " << req.get()->header->message << std::endl;
    std::cout << "Response (status): " << req.get()->header->status << std::endl;
}

void MessageHandler::SetupSocket()
{
    clientSocket = socket(AF_LOCAL, SOCK_SEQPACKET, 0);
    address.sun_family = AF_LOCAL;
    strcpy(address.sun_path, socketPath.c_str());

    int result = connect(clientSocket, reinterpret_cast<struct sockaddr*>(&address), sizeof(address));
    if(result < 0)
    {
        std::cout << "CONNECT ERROR: " << strerror(errno) << " [Is daemon running?]" << std::endl;
        close(clientSocket);
        exit(1);
    }
}

void MessageHandler::AddDaemonBlobRequest(const std::string& sender, const std::string& module, const std::string& command, const std::string &parameter, const std::vector<uint8_t>& lut_buffer)
{
    DaemonRequestT request;
    HeaderT header;
    BlobPacketT blobPacket;

    header.sender = sender;
    header.module_ = module;
    header.command = command;
    header.parameter = parameter;

    auto headerOffset = CreateHeader(*_builder, &header);

    blobPacket.value = lut_buffer;

    auto blobPacketOffset = CreateBlobPacket(*_builder, &blobPacket);

    auto req = CreateDaemonRequest(*_builder, headerOffset, PacketData::BlobPacket, blobPacketOffset.Union());
    _settings.push_back(req);
}

void MessageHandler::AddDaemonStrParamRequest(const std::string& sender, const std::string& module, const std::string& command, const std::string& parameter, const std::string& value1, const std::string& value2)
{   
    DaemonRequestT request;
    HeaderT header;
    StrParamPacketT strParamPacket;

    header.sender = sender;
    header.module_ = module;
    header.command = command;
    header.parameter = parameter;

    auto headerOffset = CreateHeader(*_builder, &header);

    strParamPacket.value1 = value1;
    strParamPacket.value2 = value2;

    auto strParamPacketOffset = CreateStrParamPacket(*_builder, &strParamPacket);

    auto req = CreateDaemonRequest(*_builder, headerOffset, PacketData::StrParamPacket, strParamPacketOffset.Union());

    _settings.push_back(req);

}

void MessageHandler::AddDaemonI2cRequest(const std::string& sender, const std::string& module, const std::string& command, const std::string &parameter, const std::string& value1, const std::string& value2, const std::string& value3, const std::string& value4)
{
    DaemonRequestT request;
    HeaderT header;
    I2cPacketT i2cPacket;

    header.sender = sender;
    header.module_ = module;
    header.command = command;
    header.parameter = parameter;

    auto headerOffset = CreateHeader(*_builder, &header);

    i2cPacket.value1 = value1;
    i2cPacket.value2 = value2;
    i2cPacket.value3 = value3;
    i2cPacket.value4 = value4;

    auto i2cPacketOffset = CreateI2cPacket(*_builder, &i2cPacket);

    auto req = CreateDaemonRequest(*_builder, headerOffset, PacketData::I2cPacket, i2cPacketOffset.Union());

    _settings.push_back(req);
}