#pragma once

#include "mqtt/async_client.h"
#include <nlohmann/json.hpp>
#include <radioSession.hpp>

using nlohmann::json;

class DeviceController {
public:
    DeviceController(
        monitor& monitor,
        mqtt::async_client& mqtt_client,
        uint32_t radioAddress,
        uint32_t gatewayAddress);

    void execute();
    void discoveryReceived(uint32_t nodeAddress);
    void setPublishBirth(bool value);
    void setPublishAdvertisement(bool value);
    void parseMessage(std::string topic, std::string message);
    uint32_t getNodeAddress();
    std::string getTopicString();
    uint32_t getLastDeviceIdSeen();

private:
    void log(std::string type, std::string message);
    void logError(std::string message);
    void logWarning(std::string message);
    void logDebug(std::string message);
    std::string createMqttTopicResponse(std::string commandName);
    std::string createMqttTopic(std::string type, std::string eon, std::string device);
    void storeValidResponse(std::string jsonResponse);
    void publishMessage(std::string topic, std::string message);
    void publishState();
    void updateQualityIndicator(bool lastCommandReturnedValidResponse);
    void executeJsonCommand();
    void publishBirth();

    json m_validJsonResponses;

    RadioSession m_radioSession;
    std::string m_topic;
    std::string m_command;
    std::string m_nodeType;
    monitor& m_monitor;
    mqtt::async_client& m_mqttClient;
    int healthIndicator = 0;
    uint64_t timestampLastDiscovery = 0;
    uint32_t m_radioAddress = 0;
    uint32_t m_gatewayAddress = 0;
    uint32_t m_lastDeviceIdSeen = 0;
    bool m_publishBirth = false;
    bool m_publishAdvertisement = false;
    bool m_commandReceived = false;
};
