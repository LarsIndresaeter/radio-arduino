#include "commandCallback.hpp"

using nlohmann::json;

CommandCallback::CommandCallback() {}

void CommandCallback::addDeviceController(std::shared_ptr<DeviceController> dc) { m_deviceControllerList.push_back(dc); }

void CommandCallback::message_arrived(mqtt::const_message_ptr message)
{
    if (message->get_topic() == "radio-arduino/RCMD") {
        for (int i = 0; i < m_deviceControllerList.size(); i++) {
            std::shared_ptr<DeviceController> dc = m_deviceControllerList.at(i);
            dc->setPublishBirth(true);
        }
    }

    for (int i = 0; i < m_deviceControllerList.size(); i++) {
        std::shared_ptr<DeviceController> dc = m_deviceControllerList.at(i);
        if (dc->getTopicString() == message->get_topic()) {
            dc->parseMessage(message->get_topic(), message->get_payload_str());
        }
    }
}

void CommandCallback::connection_lost(const std::string& cause)
{
    std::cout << "Connection lost: " << cause << std::endl;
}

void CommandCallback::delivery_complete(mqtt::delivery_token_ptr token)
{
    std::cout << "Message delivered" << std::endl;
}
