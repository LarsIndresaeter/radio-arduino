#pragma once

#include "mqtt/async_client.h"
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

typedef struct nodepath
{
    std::string gatewayName;
    uint32_t nodeAddress;
    uint64_t lastSeen;
    int healthIndicator;
} nodepath_t;

class CommandCallback : public virtual mqtt::callback {
public:
    CommandCallback(

        mqtt::async_client& mqtt_client);
    void connection_lost(const std::string& cause) override;
    void message_arrived(mqtt::const_message_ptr message) override;
    void delivery_complete(mqtt::delivery_token_ptr token) override;

private:
    void publishMessage(std::string topic, std::string message);
    mqtt::async_client& m_mqttClient;
    void updatePath(std::string gatewayName, uint32_t nodeAddress, uint64_t lastSeen, int healthIndicator);
    nodepath_t getNodePath(uint32_t nodeAddress);
    std::vector<nodepath_t> m_nodePath;
};
