#pragma once

#include <monitor.hpp>

class RadioSession {
public:
    RadioSession(monitor& mon, uint8_t address);
    void close();
    uint64_t getActiveTimeForRadioNode();
    bool wakeupNotResponding();
    uint32_t getWakeupSuccessCounter();
    uint32_t getWakeupFailedCounter();
    void setKeepAliveInterval(uint8_t interval);
    void setVerbose(bool verbose);
    std::string getNodeName(monitor& mon);
private:
    bool wakeupNotRespondingTryOnce();
    monitor& m_monitor;
    uint8_t m_radioAddress;
    uint8_t m_keepAliveInterval;
    uint8_t m_initialKeepAliveInterval;
    uint8_t m_wakeupAttempts;
    bool m_isAlive; 
    uint32_t m_wakeupSuccessCounter;
    uint32_t m_wakeupFailedCounter;
    uint64_t m_timeLastWakeup;
    uint64_t m_activeTime;
    uint64_t milliSecondsSinceEpoch();
    uint64_t secondsSinceEpoch();
    bool m_verbose;
};

