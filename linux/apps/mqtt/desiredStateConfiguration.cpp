#include "desiredStateConfiguration.hpp"
#include "mqtt_common.hpp"

using nlohmann::json;

DesiredStateConfiguration::DesiredStateConfiguration(uint8_t radioAddress, std::string name)
    : m_radioAddress(radioAddress)
    , m_name(name)
{
    m_pollInterval = 1800;
    m_topic = createMqttTopic("RCMD", m_name, "");
}

void DesiredStateConfiguration::parseMessage(std::string topic, std::string message)
{
    // std::cout << "DEBUG: got message on topic: " << topic << std::endl;
    auto jsonData = json::parse(message);

    if (topic == m_topic) {
        std::string commandParamString = jsonData["command"];

        if (!commandParamString.empty()) {
            if (commandParamString == "desiredState") {
                try {
                    m_pollInterval = jsonData["pollInterval"];
                }
                catch (std::exception const& e) {
                    // std::cout << "DEBUG: exception" << std::endl;
                }
            }
            else if (commandParamString == "lcd") {
                m_desiredDisplayText = jsonData["displayText"];
                m_displayTextChanged = true;
            }
        }
    }
}

std::string DesiredStateConfiguration::getTopicString()
{
    return (m_topic);
}

std::string DesiredStateConfiguration::getName()
{
    return (m_name);
}

bool DesiredStateConfiguration::getStateSubscribeVoltage()
{
    return (true);
}

uint8_t DesiredStateConfiguration::getRadioAddress()
{
    return (m_radioAddress);
}

int DesiredStateConfiguration::getPollInterval()
{
    return (m_pollInterval);
}

std::string DesiredStateConfiguration::getDesiredDisplayText()
{
    return (m_desiredDisplayText);
}

void DesiredStateConfiguration::setActualDisplayText()
{
    m_displayTextChanged = false;
    // std::cout << "DEBUG: update actual state" << std::endl;
}

bool DesiredStateConfiguration::displayTextChanged()
{
    return (m_displayTextChanged);
}

//////////////////////////////////

DesiredState::DesiredState(std::shared_ptr<DesiredStateConfiguration> dsc)
    : m_desiredStateConfiguration(dsc)
{
}

void DesiredState::message_arrived(mqtt::const_message_ptr message)
{
    m_desiredStateConfiguration->parseMessage(message->get_topic(), message->get_payload_str());
}

void DesiredState::connection_lost(const std::string& cause)
{
    std::cout << "Connection lost: " << cause << std::endl;
}

void DesiredState::delivery_complete(mqtt::delivery_token_ptr token)
{
    std::cout << "Message delivered" << std::endl;
}

