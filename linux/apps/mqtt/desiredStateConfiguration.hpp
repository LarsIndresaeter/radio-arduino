#pragma once

#include "mqtt/async_client.h"
#include <nlohmann/json.hpp>

class ActualState {
public:
    ActualState();
    void setTimeLastPoll(uint64_t time);
    uint64_t getTimeLastPoll();

    void setActualPollInterval(int interval);
    int getActualPollInterval();
    void setActualDisplayText(std::string displayText);
    std::string getActualDisplayText();

private:
    int m_actualPollInterval;
    std::string m_actualDisplayText;
};

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

class DesiredStateCallback : public virtual mqtt::callback {
public:
    DesiredStateCallback(std::vector<std::shared_ptr<DesiredStateConfiguration>> dscList);
    void addDesiredStateConfiguration(std::shared_ptr<DesiredStateConfiguration> dsc);
    void connection_lost(const std::string& cause) override;
    void message_arrived(mqtt::const_message_ptr message) override;
    void delivery_complete(mqtt::delivery_token_ptr token) override;

private:
    std::vector<std::shared_ptr<DesiredStateConfiguration>> m_desiredStateConfiguration;
};

