#include "commandCallback.hpp"
#include "include/commandCallback.hpp"

using nlohmann::json;

CommandCallback::CommandCallback() {}

void CommandCallback::addDeviceController(std::shared_ptr<DeviceController> dc)
{
    m_deviceControllerList.push_back(dc);
}

void CommandCallback::message_arrived(mqtt::const_message_ptr message)
{
    if (message->get_topic() == "raduino-adapter/RCMD") {
        for (int i = 0; i < m_deviceControllerList.size(); i++) {
            m_deviceControllerList.at(i)->setPublishBirth(true);
        }
    }

    // command for node
    for (int i = 0; i < m_deviceControllerList.size(); i++) {
        if (m_deviceControllerList.at(i)->getTopicString() == message->get_topic()) {
            m_deviceControllerList.at(i)->parseMessage(message->get_topic(), message->get_payload_str());
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
