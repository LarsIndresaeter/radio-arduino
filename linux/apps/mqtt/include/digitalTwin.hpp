#pragma once

#include <desiredState.hpp>
#include <actualState.hpp>
#include <radioSession.hpp>
#include "mqtt/async_client.h"
#include <mqtt_common.hpp>

class DigitalTwin {
public:
    DigitalTwin(monitor& monitor, mqtt::async_client& mqtt_client, uint8_t radioAddress, std::string name);
    void execute();
    std::shared_ptr<DesiredState> getDesiredState();

private:
    void publishMessage(std::string topic, std::string message);
    void reconsileState();
    ActualState m_actualState;
    std::shared_ptr<DesiredState> m_desiredState;
    RadioSession m_radioSession;
    std::string m_name;
    uint8_t m_radioAddress;
    monitor& m_monitor;
    mqtt::async_client& m_mqttClient;

    bool readVccAndPublish();
    void readGpioAndPublish();
    void updateDisplayText();

    void publishNdeath();
    uint64_t m_timeLastPoll = 0;
};
