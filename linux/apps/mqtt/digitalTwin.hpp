#pragma once

#include "desiredStateConfiguration.hpp"
#include "radioSession.hpp"
#include "mqtt/async_client.h"
#include "mqtt_common.hpp"

class DigitalTwin {
public:
    DigitalTwin(monitor& monitor, mqtt::async_client& mqtt_client, uint8_t radioAddress, std::string name);
    void execute();
    std::shared_ptr<DesiredStateConfiguration> getDesiredStateConfiguration();
private:
    ActualState m_actualState;
    std::shared_ptr<DesiredStateConfiguration> m_desiredStateConfiguration;
    RadioSession m_radioSession;
    std::string m_name;

    uint8_t m_radioAddress;
    monitor& m_monitor;
    mqtt::async_client& m_mqttClient;

    void readVccAndPublish();
    void updateDisplayText();

    void publishDesiredStatePollInterval();
    void publishActualStateDisplayText(std::string displayText);
    void publishVcc(std::string voltage);
    void publishNdeath();
    uint64_t m_timeLastPoll = 0;
};
