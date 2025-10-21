#include "actualState.hpp"
#include "mqtt-common.hpp"

using nlohmann::json;

// constexpr int pollInterval=3600;

ActualState::ActualState()
    : m_actualDisplayText("")
    , m_actualPollInterval(3600)
{
}

void ActualState::setActualDisplayText(std::string displayText) { m_actualDisplayText = displayText; }

std::string ActualState::getActualDisplayText() { return (m_actualDisplayText); }

int ActualState::getActualPollInterval() { return (m_actualPollInterval); }

void ActualState::setActualPollInterval(int interval) { m_actualPollInterval = interval; }
