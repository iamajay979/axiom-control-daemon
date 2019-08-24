#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include "IMessageHandler.h"
#include "MessageDefines.h"

#include <sys/un.h>
#include <sys/socket.h>

#include <Schema/axiom_daemon_generated.h>

#define BUFFER_MAX 163840
namespace ns
{
struct JSONSetting;
}

class MessageHandler : public IMessageHandler
{
    std::string socketPath;
    int clientSocket;
    struct sockaddr_un address;
    socklen_t _sockaddrLength;

    // TODO (BAndiT1983): Rework, possibly shrink it, as the data is much smaller currently
    char _response[BUFFER_MAX];
    
    // Using separate lists for now as it seems that flatbuffers does not use inheritance for unions
    std::vector<flatbuffers::Offset<DaemonRequest>> _settings;

    //std::vector<const ImageSensorSetting*> _settingsIS;

    flatbuffers::FlatBufferBuilder* _builder = nullptr;

public:
    MessageHandler();
    ~MessageHandler() override;

    // Process JSON message and return response
    virtual bool ProcessMessage(std::string message, std::string& response) override;

    void SetupSocket();

    void Execute();
    void TransferData(std::unique_ptr<DaemonRequestT>& req);
    std::vector<unsigned char> Base64Decode(std::string const& encoded_string);
    bool IsBase64(unsigned char c);

    void AddDaemonStrParamRequest(const std::string& sender, const std::string& module, const std::string& command, const std::string &parameter, const std::string& value1, const std::string& value2 = nullptr);
    void AddDaemonBlobRequest(const std::string& sender, const std::string& module, const std::string& command, const std::string &parameter, const std::vector<uint8_t>& lut_buffer);
    void AddDaemonI2cRequest(const std::string& sender, const std::string& module, const std::string& command, const std::string &parameter, const std::string& value1, const std::string& value2, const std::string& value3, const std::string& value4);
    void OutputReceivedData(ns::JSONSetting setting, std::string& message);
};

#endif //MESSAGEHANDLER_H
