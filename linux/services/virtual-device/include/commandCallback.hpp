#pragma once

#include "mqtt/async_client.h"
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

using nlohmann::json;

class CommandCallback : public virtual mqtt::callback {
public:
    CommandCallback(

        mqtt::async_client& mqtt_client);
    void connection_lost(const std::string& cause) override;
    void message_arrived(mqtt::const_message_ptr message) override;
    void delivery_complete(mqtt::delivery_token_ptr token) override;

private:
    std::vector<std::string> splitString(std::string s, const std::string& delimiter);
    void publishMessage(std::string topic, std::string message);
    mqtt::async_client& m_mqttClient;

    json devices;
};
