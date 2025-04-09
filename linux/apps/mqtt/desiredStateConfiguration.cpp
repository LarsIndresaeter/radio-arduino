#include "desiredStateConfiguration.hpp"
#include "mqtt_common.hpp"

using nlohmann::json;

ActualState::ActualState()
{
    m_timeLastPoll=0;
}

void ActualState::setTimeLastPoll(uint64_t time)
{
    m_timeLastPoll = time;
}

uint64_t ActualState::getTimeLastPoll()
{
    return(m_timeLastPoll);
}

////////////////////////////////////////////


DesiredStateConfiguration::DesiredStateConfiguration(uint8_t radioAddress, std::string name)
    : m_radioAddress(radioAddress)
    , m_name(name)
{
    m_desiredPollInterval = 3600;
    m_actualPollInterval = m_desiredPollInterval;
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
                    m_desiredPollInterval = jsonData["pollInterval"];
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

void DesiredStateConfiguration::setTimeLastPoll(uint64_t time)
{
    m_timeLastPoll = time;
}

uint64_t DesiredStateConfiguration::getTimeLastPoll()
{
    return(m_timeLastPoll);
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

int DesiredStateConfiguration::getDesiredPollInterval()
{
    return (m_desiredPollInterval);
}

int DesiredStateConfiguration::getActualPollInterval()
{
    return (m_actualPollInterval);
}

void DesiredStateConfiguration::setActualPollInterval(int interval)
{
    m_actualPollInterval = interval;
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

DesiredStateCallback::DesiredStateCallback(std::vector<std::shared_ptr<DesiredStateConfiguration>> dscList)
{
    for (int i = 0; i < dscList.size(); i++) {
        m_desiredStateConfiguration.push_back(dscList.at(i));
    }
}

void DesiredStateCallback::addDesiredStateConfiguration(std::shared_ptr<DesiredStateConfiguration> dsc)
{
    m_desiredStateConfiguration.push_back(dsc);
}

void DesiredStateCallback::message_arrived(mqtt::const_message_ptr message)
{
    //std::cout << "DEBUG: got message on topic: " << message->get_topic() << std::endl;
    for (int i = 0; i < m_desiredStateConfiguration.size(); i++) {
        std::shared_ptr<DesiredStateConfiguration> dsc = m_desiredStateConfiguration.at(i);
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

