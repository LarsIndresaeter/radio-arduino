#pragma once

#include <monitor.hpp>

class RadioSession {
public:
    RadioSession(monitor& mon, uint8_t address);
    ~RadioSession();
    bool wakeupNotResponding();
    void setKeepAliveInterval(uint8_t interval);
    std::string readSlaveName(monitor& mon);
private:
    bool wakeupNotRespondingTryOnce();
    monitor& m_monitor;
    uint8_t m_radioAddress;
    uint8_t m_keepAliveInterval;
    uint8_t m_wakeupAttempts;
    //std::shared_ptr<monitor> m_monitor;
};

