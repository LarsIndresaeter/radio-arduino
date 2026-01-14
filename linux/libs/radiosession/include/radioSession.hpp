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
    void setVerbose(bool verbose);

private:
    bool wakeupNotRespondingTryOnce();
    monitor& m_monitor;
    uint8_t m_keepAliveInterval;
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
