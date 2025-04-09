#pragma once

#include "mqtt/async_client.h"
#include <nlohmann/json.hpp>

class ActualState {
public:
    ActualState();
    void setTimeLastPoll(uint64_t time);
    uint64_t getTimeLastPoll();

    void setActualPollInterval(int interval);
    int getActualPollInterval();
    void setActualDisplayText(std::string displayText);
    std::string getActualDisplayText();

private:
    int m_actualPollInterval;
    std::string m_actualDisplayText;
};

