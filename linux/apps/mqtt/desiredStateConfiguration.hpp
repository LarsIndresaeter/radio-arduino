#pragma once

#include "mqtt/async_client.h"
#include <nlohmann/json.hpp>

class DesiredStateConfiguration {
public:
    DesiredStateConfiguration(uint8_t radioAddress, std::string name);
    void parseMessage(std::string topic, std::string message);

    bool getStateSubscribeVoltage();
    int getDesiredPollInterval();
    std::string getDesiredDisplayText();
    std::string getName();
    std::string getTopicString();

private:
    std::string m_desiredDisplayText;
    int m_desiredPollInterval;
    std::string m_name;
    std::string m_topic;
};

