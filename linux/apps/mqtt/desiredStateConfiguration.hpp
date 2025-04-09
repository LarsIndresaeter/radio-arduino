#pragma once

#include "mqtt/async_client.h"
#include <nlohmann/json.hpp>

class ActualState {
public:
    ActualState();
    void setTimeLastPoll(uint64_t time);
    uint64_t getTimeLastPoll();

private:
    uint64_t m_timeLastPoll = 0;
};

class DesiredStateConfiguration {
public:
    DesiredStateConfiguration(uint8_t radioAddress, std::string name);
    void parseMessage(std::string topic, std::string message);

    bool getStateSubscribeVoltage();
    uint8_t getRadioAddress();
    int getActualPollInterval();
    int getDesiredPollInterval();
    void setActualPollInterval(int interval);
    std::string getDesiredDisplayText();
    std::string getName();
    void setActualDisplayText();
    bool displayTextChanged();
    std::string getTopicString();
    void setTimeLastPoll(uint64_t time);
    uint64_t getTimeLastPoll();

private:
    // std::vector<uint8_t> m_slaveAddressList;
    std::string m_desiredDisplayText;
    bool m_displayTextChanged = false;
    int m_desiredPollInterval;
    int m_actualPollInterval;
    uint8_t m_radioAddress;
    uint64_t m_timeLastPoll = 0;
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

