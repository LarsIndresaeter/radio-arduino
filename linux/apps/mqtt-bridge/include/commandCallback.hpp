#pragma once

#include "mqtt/async_client.h"
#include <nlohmann/json.hpp>
#include <deviceController.hpp>

class CommandCallback : public virtual mqtt::callback {
public:
    CommandCallback();
    void addDeviceController(std::shared_ptr<DeviceController> dsc);
    void connection_lost(const std::string& cause) override;
    void message_arrived(mqtt::const_message_ptr message) override;
    void delivery_complete(mqtt::delivery_token_ptr token) override;

private:
    std::vector<std::shared_ptr<DeviceController>> m_deviceControllerList;
};
