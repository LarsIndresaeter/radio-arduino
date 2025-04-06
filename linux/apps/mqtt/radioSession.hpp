#pragma once

#include <monitor.hpp>

class RadioSession {
public:
    //RadioSession(monitor& mon);
    RadioSession(uint8_t address);
private:
    uint8_t m_radioAddress;
    //std::shared_ptr<monitor> m_monitor;
};

