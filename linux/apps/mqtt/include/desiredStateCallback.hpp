#pragma once

#include <desiredState.hpp>
#include "mqtt/async_client.h"
#include <nlohmann/json.hpp>

class DesiredStateCallback : public virtual mqtt::callback {
public:
    DesiredStateCallback(std::vector<std::shared_ptr<DesiredState>> dscList);
    void addDesiredState(std::shared_ptr<DesiredState> dsc);
    void connection_lost(const std::string& cause) override;
    void message_arrived(mqtt::const_message_ptr message) override;
    void delivery_complete(mqtt::delivery_token_ptr token) override;

private:
    std::vector<std::shared_ptr<DesiredState>> m_desiredState;
};

