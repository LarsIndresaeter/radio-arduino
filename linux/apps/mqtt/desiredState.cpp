#include "desiredState.hpp"
#include "mqtt-common.hpp"

using nlohmann::json;

// constexpr int pollInterval=3600;

DesiredState::DesiredState(uint32_t radioAddress, std::string name)
    : m_name(name)
    , m_desiredPollInterval(3600)
    , m_desiredDisplayText("")
{
    m_topic = createMqttTopic("RCMD", m_name, "");
}

void DesiredState::parseMessage(std::string topic, std::string message)
{
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
            }
        }
    }
}

std::string DesiredState::getTopicString() { return (m_topic); }

std::string DesiredState::getName() { return (m_name); }

bool DesiredState::getStateSubscribeVoltage() { return (true); }

int DesiredState::getDesiredPollInterval() { return (m_desiredPollInterval); }

std::string DesiredState::getDesiredDisplayText() { return (m_desiredDisplayText); }
