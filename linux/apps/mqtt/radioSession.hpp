#pragma once

#include <monitor.hpp>

class RadioSession {
public:
    //RadioSession(monitor& mon);
    RadioSession(uint8_t address);
    bool wakeupNotResponding(monitor& mon, uint8_t address, uint8_t attempts);
private:
    bool wakeupNotRespondingTryOnce(monitor& mon, uint8_t address);
    uint8_t m_radioAddress;
    //std::shared_ptr<monitor> m_monitor;
};

