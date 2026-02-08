#pragma once

#include "mqtt/async_client.h"
#include <desiredState.hpp>
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
    std::shared_ptr<DesiredState> getDesiredState();
    uint32_t getNodeAddress();
    void discoveryReceived();
    void updateHealthScore(int newHealthScore);

private:
    std::string createMqttTopicResponse(std::string commandName);
    std::string createMqttTopic(std::string type, std::string eon, std::string device);
    void publishMessage(std::string topic, std::string message);
    std::shared_ptr<DesiredState> m_desiredState;
    RadioSession m_radioSession;
    std::string m_nodeName;
    std::string m_gatewayName;
    uint32_t m_radioAddress;
    monitor& m_monitor;
    mqtt::async_client& m_mqttClient;
    int healthScore;
    uint64_t timestampLastDiscovery;

    void executeJsonCommand(std::string command);
};
