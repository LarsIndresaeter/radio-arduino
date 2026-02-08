#include "desiredState.hpp"
#include "include/desiredState.hpp"

using nlohmann::json;

DesiredState::DesiredState(uint32_t radioAddress, std::string nodeName, std::string gatewayName)
    : m_nodeName(nodeName)
    , m_gatewayName(gatewayName)
    , m_radioAddress(radioAddress)
{
    m_topic = createMqttTopic("RCMD", m_gatewayName, std::to_string(m_radioAddress));
}

void DesiredState::parseMessage(std::string topic, std::string message)
{
    try {
        auto jsonData = json::parse(message);

        if (topic == m_topic) {
            if (!m_commandReceived) {
                m_desiredCommand = message;
                m_commandReceived = true;
            }
        }
    }
    catch (const mqtt::exception& exc) {
        std::cerr << exc.what() << std::endl;
    }
}

bool DesiredState::publishBirth() { return m_publishBirth; }

void DesiredState::setPublishBirth(bool value) { m_publishBirth = value; }

void DesiredState::clearDesiredCommand()
{
    m_desiredCommand = "";
    m_commandReceived = false;
}

bool DesiredState::hasNewCommand() { return (m_commandReceived); }

std::string DesiredState::getTopicString() { return (m_topic); }

std::string DesiredState::getDesiredCommand() { return (m_desiredCommand); }

std::string DesiredState::createMqttTopic(std::string type, std::string eon, std::string device)
{
    std::string retval("radio-arduino/" + type + "/" + eon);
    if (!device.empty()) {
        retval.append("/" + device);
    }
    return (retval);
}

