#pragma once

#include <monitor.hpp>
#include "mqtt/async_client.h"
#include <mqtt_common.hpp>
#include <desiredStateConfiguration.hpp>

class RadioSession {
public:
    RadioSession(monitor& mon, uint8_t address);
    ~RadioSession();
    bool wakeupNotResponding();
    void setKeepAliveInterval(uint8_t interval);
    std::string readSlaveName(monitor& mon);
    std::string getSlaveNameAndPublishBirth(mqtt::async_client& mqtt_client);
private:
    bool wakeupNotRespondingTryOnce();
    monitor& m_monitor;
    uint8_t m_radioAddress;
    uint8_t m_keepAliveInterval;
    uint8_t m_wakeupAttempts;
    bool m_isAlive; 
    //std::shared_ptr<monitor> m_monitor;
};

