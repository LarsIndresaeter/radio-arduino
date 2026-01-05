#pragma once

#include "mqtt/async_client.h"
#include <actualState.hpp>
#include <desiredState.hpp>
#include <mqtt-common.hpp>
#include <radioSession.hpp>

class DeviceController {
public:
    DeviceController(monitor& monitor, mqtt::async_client& mqtt_client, uint8_t radioAddress, std::string name);
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
    std::string m_attachedDevicesCsvString;

    bool hasDeviceAttached(std::string device);
    void readAttachedDevicesCsvString();
    void readVccAndPublish();
    void readGpioAndPublish();
    void readQuadencoderAndPublish();
    void readAccelerometerAndPublish();
    void readUniqueuIdAndPublish();
    void readVersionAndPublish();
    void readAttachedDevicesAndPublish();
    void updateDisplayText();

    void publishNdeath();
    uint64_t m_timeLastPoll = 0;
};
