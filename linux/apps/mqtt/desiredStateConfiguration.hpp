#pragma once

#include "mqtt/async_client.h"
#include <nlohmann/json.hpp>

class DesiredStateConfiguration{
public:
    DesiredStateConfiguration(uint8_t radioAddress, std::string name);
    void parseMessage(std::string topic, std::string message);

    bool getStateSubscribeVoltage();
    uint8_t getRadioAddress();
    int getPollInterval();
    std::string getDesiredDisplayText();
    std::string getName();
    void setActualDisplayText();
    bool displayTextChanged();
    std::string getTopicString();

private:
    //std::vector<uint8_t> m_slaveAddressList;
    std::string m_desiredDisplayText;
    bool m_displayTextChanged = false;
    int m_pollInterval;
    uint8_t m_radioAddress;
    uint64_t timeLastPublish=0;
    std::string m_name;
    std::string m_topic;
};

class DesiredState : public virtual mqtt::callback
{
public:
    DesiredState(std::shared_ptr<DesiredStateConfiguration> dsc); // seher
    void connection_lost(const std::string& cause) override;
    void message_arrived(mqtt::const_message_ptr message) override;
    void delivery_complete(mqtt::delivery_token_ptr token) override;

    private:
    std::shared_ptr<DesiredStateConfiguration> m_desiredStateConfiguration;
};


