#include "commandCallback.hpp"

using nlohmann::json;

CommandCallback::CommandCallback(std::vector<std::shared_ptr<DesiredState>> dscList)
{
    for (int i = 0; i < dscList.size(); i++) {
        m_desiredState.push_back(dscList.at(i));
    }
}

void CommandCallback::addDesiredState(std::shared_ptr<DesiredState> dc) { m_desiredState.push_back(dc); }

void CommandCallback::message_arrived(mqtt::const_message_ptr message)
{
    // TODO:
    //  call gateway desired state
    if (message->get_topic() == "radio-arduino/RCMD") {
        for (int i = 0; i < m_desiredState.size(); i++) {
            std::shared_ptr<DesiredState> dc = m_desiredState.at(i);
            dc->setPublishBirth(true);
        }
    }

    // std::cout << "DEBUG: got message on topic: " << message->get_topic() << std::endl;
    for (int i = 0; i < m_desiredState.size(); i++) {
        std::shared_ptr<DesiredState> dc = m_desiredState.at(i);
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
