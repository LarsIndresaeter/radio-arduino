#pragma once

#include "mqtt/async_client.h"
#include <nlohmann/json.hpp>

class DesiredState {
public:
    DesiredState(uint32_t radioAddress, std::string nodeName, std::string gatewayName);
    void parseMessage(std::string topic, std::string message);

    std::string getDesiredCommand();
    std::string getTopicString();
    bool hasNewCommand();
    void clearDesiredCommand();
    void setPublishBirth(bool value);
    bool publishBirth();

private:
    std::string createMqttTopic(std::string type, std::string eon, std::string device);
    std::string m_desiredCommand;
    bool m_commandReceived;
    std::string m_nodeName;
    std::string m_gatewayName;
    uint32_t m_radioAddress;
    std::string m_topic;
    bool m_publishBirth = false;
};
