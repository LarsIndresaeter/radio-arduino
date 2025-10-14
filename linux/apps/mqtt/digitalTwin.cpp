#include "digitalTwin.hpp"

DigitalTwin::DigitalTwin(monitor& monitor, mqtt::async_client& mqtt_client, uint8_t radioAddress, std::string name)
    : m_radioAddress(radioAddress)
    , m_monitor(monitor)
    , m_radioSession(monitor, radioAddress)
    , m_desiredState(std::make_shared<DesiredState>(radioAddress, name))
    , m_mqttClient(mqtt_client)
    , m_name(name)
{
}

std::shared_ptr<DesiredState> DigitalTwin::getDesiredState()
{
    return (m_desiredState);
}

void DigitalTwin::reconsileState()
{
    if (m_desiredState->getDesiredPollInterval() != m_actualState.getActualPollInterval()) {
        m_actualState.setActualPollInterval(m_desiredState->getDesiredPollInterval());

        std::string topic = createMqttTopic("STATE", m_name, "actualState");
        std::string message
            = "{\"dateString\": \"" + getDateTimeString() + "\", \"pollInterval\":" + std::to_string(m_actualState.getActualPollInterval()) + "}";

        publishMessage(topic, message);
    }
}

void DigitalTwin::execute()
{
    reconsileState();

    if ((secondsSinceEpoch() - m_timeLastPoll) > m_actualState.getActualPollInterval()) {
        if (m_radioSession.wakeupNotResponding()) {
            readVccAndPublish();
            readGpioAndPublish();
            m_timeLastPoll = secondsSinceEpoch();
        }
    }

    if (m_desiredState->getDesiredDisplayText() != m_actualState.getActualDisplayText()) {
        if (m_radioSession.wakeupNotResponding()) {
            updateDisplayText();
            m_actualState.setActualDisplayText(m_desiredState->getDesiredDisplayText());
        }
    }

    // m_radioSession.close();
}

void DigitalTwin::publishMessage(std::string topic, std::string message)
{
    const int QOS = 0;
    mqtt::topic mqttTopic(m_mqttClient, topic, QOS, false);

    try {
        mqttTopic.publish(std::move(message));
    }
    catch (const mqtt::exception& exc) {
        std::cerr << exc.what() << std::endl;
        std::cerr << "topic: " << topic << std::endl;
        std::cerr << "message: " << message << std::endl;
    }
}

void DigitalTwin::readVccAndPublish()
{
    auto response = m_monitor.getRadio<>(RaduinoCommandVcc());

    if (m_monitor.lastCommandReturnedValidResponse()) {
        std::string topic = createMqttTopic("NDATA", m_name, response.getCommandName());
        publishMessage(topic, response.getJson());
    }
}

void DigitalTwin::readGpioAndPublish()
{
    auto response = m_monitor.getRadio<>(RaduinoCommandGpio());

    if (m_monitor.lastCommandReturnedValidResponse()) {
        std::string topic = createMqttTopic("NDATA", m_name, response.getCommandName());
        publishMessage(topic, response.getJson());
    }
}

void DigitalTwin::updateDisplayText()
{
    COMMANDS::SSD1306::command_t command;
    std::vector<uint8_t> lcd(sizeof(command.data), ' ');
    std::string displayText = m_desiredState->getDesiredDisplayText();

    for (uint8_t i = 0; i < displayText.size() && i < static_cast<uint8_t>(sizeof(command.data)); i++) {
        lcd.at(i) = displayText.at(i);
    }

    auto response = m_monitor.getRadio<>(RaduinoCommandSsd1306(2, lcd), static_cast<std::chrono::milliseconds>(500));
    if (m_monitor.lastCommandReturnedValidResponse()) {

        std::string topic = m_desiredState->getTopicString() + "/actualState";
        std::string message
            = "{\"dateString\": \"" + getDateTimeString() + "\", \"displayText\": \"" + displayText + "\"}";

        publishMessage(topic, message);
    }
}

void DigitalTwin::publishNdeath()
{
    std::string topic = createMqttTopic("NDEATH", m_name, "");
    std::string message = getDateTimeString();
    publishMessage(topic, message);
}

