#include "desiredStateCallback.hpp"
#include "mqtt-common.hpp"

using nlohmann::json;

DesiredStateCallback::DesiredStateCallback(std::vector<std::shared_ptr<DesiredState>> dscList)
{
    for (int i = 0; i < dscList.size(); i++) {
        m_desiredState.push_back(dscList.at(i));
    }
}

void DesiredStateCallback::addDesiredState(std::shared_ptr<DesiredState> dsc) { m_desiredState.push_back(dsc); }

void DesiredStateCallback::message_arrived(mqtt::const_message_ptr message)
{
    // std::cout << "DEBUG: got message on topic: " << message->get_topic() << std::endl;
    for (int i = 0; i < m_desiredState.size(); i++) {
        std::shared_ptr<DesiredState> dsc = m_desiredState.at(i);
        if (dsc->getTopicString() == message->get_topic()) {
            dsc->parseMessage(message->get_topic(), message->get_payload_str());
        }
    }
}

void DesiredStateCallback::connection_lost(const std::string& cause)
{
    std::cout << "Connection lost: " << cause << std::endl;
}

void DesiredStateCallback::delivery_complete(mqtt::delivery_token_ptr token)
{
    std::cout << "Message delivered" << std::endl;
}
