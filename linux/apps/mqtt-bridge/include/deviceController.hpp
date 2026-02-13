#pragma once

#include "mqtt/async_client.h"
#include <nlohmann/json.hpp>
#include <radioSession.hpp>

class DeviceController {
public:
    DeviceController(
        monitor& monitor,
        mqtt::async_client& mqtt_client,
        uint32_t radioAddress,
        std::string nodeName,
        std::string gatewayName);

    void execute();
    void discoveryReceived(uint32_t nodeAddress);
    void setPublishBirth(bool value);
    void parseMessage(std::string topic, std::string message);
    uint32_t getNodeAddress();
    std::string getTopicString();

private:
    std::string createMqttTopicResponse(std::string commandName);
    std::string createMqttTopic(std::string type, std::string eon, std::string device);
    void publishMessage(std::string topic, std::string message);
    void publishState();
    void updateQualityIndicator(bool lastCommandReturnedValidResponse);
    void executeJsonCommand();

    RadioSession m_radioSession;
    std::string m_nodeName;
    std::string m_gatewayName;
    std::string m_topic;
    std::string m_command;
    monitor& m_monitor;
    mqtt::async_client& m_mqttClient;
    int healthIndicator = 0;
    uint64_t timestampLastDiscovery = 0;
    uint32_t m_radioAddress = 0;
    bool m_publishBirth = false;
    bool m_commandReceived = false;
};
