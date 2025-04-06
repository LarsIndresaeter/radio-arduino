#include "radioSession.hpp"
#include "desiredStateConfiguration.hpp"

//RadioSession::RadioSession(monitor& mon) : m_monitor(std::make_shared<monitor>(mon))
RadioSession::RadioSession(uint8_t address) : m_radioAddress(address)
{
};
