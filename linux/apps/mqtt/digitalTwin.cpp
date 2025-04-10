#include "digitalTwin.hpp"

DigitalTwin::DigitalTwin(monitor& monitor, mqtt::async_client& mqtt_client, uint8_t radioAddress, std::string name)
    : m_radioAddress(radioAddress)
    , m_monitor(monitor)
    , m_radioSession(monitor, radioAddress)
    , m_desiredStateConfiguration(std::make_shared<DesiredStateConfiguration>(radioAddress, name))
    , m_mqttClient(mqtt_client)
    , m_name(name)
{
}

std::shared_ptr<DesiredStateConfiguration> DigitalTwin::getDesiredStateConfiguration()
{
    return (m_desiredStateConfiguration);
}

void DigitalTwin::execute()
{
    if (m_desiredStateConfiguration->getDesiredPollInterval() != m_actualState.getActualPollInterval()) {
        m_actualState.setActualPollInterval(m_desiredStateConfiguration->getDesiredPollInterval());
        publishDesiredStatePollInterval();
    }

    if ((secondsSinceEpoch() - m_timeLastPoll) > m_actualState.getActualPollInterval()) {
        if (m_radioSession.wakeupNotResponding()) {
            m_timeLastPoll = secondsSinceEpoch();
            readVccAndPublish();
        }
    }

    if (m_desiredStateConfiguration->getDesiredDisplayText() != m_actualState.getActualDisplayText()) {
        if (m_radioSession.wakeupNotResponding()) {
            m_radioSession.setKeepAliveInterval(50);
            updateDisplayText();
            m_actualState.setActualDisplayText(m_desiredStateConfiguration->getDesiredDisplayText());
        }
    }

    m_radioSession.close();
}

void DigitalTwin::readVccAndPublish()
{
    auto slaveVcc = m_monitor.getRadio<>(UartCommandVcc());

    if (slaveVcc.getReplyStatus() == UartCommandBase::ReplyStatus::Complete) {
        uint16_t vcc_mv = (uint16_t)(slaveVcc.responseStruct().vcc_h << 8)
            | slaveVcc.responseStruct().vcc_l;
        publishVcc(std::to_string(vcc_mv / 1000.0));
    }
    else {
        publishNdeath();
    }
}

void DigitalTwin::updateDisplayText()
{
    std::vector<uint8_t> lcd(COMMANDS::SSD1306::STRING_LENGTH, ' ');
    std::string displayText = m_desiredStateConfiguration->getDesiredDisplayText();

    for (uint8_t i = 0; i < displayText.size() && i < COMMANDS::SSD1306::STRING_LENGTH; i++) {
        lcd.at(i) = displayText.at(i);
    }

    auto response = m_monitor.getRadio<>(UartCommandSsd1306(2, lcd), static_cast<std::chrono::milliseconds>(500));
    if (response.getReplyStatus() == UartCommandBase::ReplyStatus::Complete) {
        publishActualStateDisplayText(displayText);
    }
}

void DigitalTwin::publishDesiredStatePollInterval()
{
    const int QOS = 0;

    mqtt::topic actual_state_topic(m_mqttClient, createMqttTopic("STATE", m_name, "actualState"), QOS, false);
    std::string mqtt_payload
        = "{\"dateString\": \"" + getDateTimeString() + "\", \"pollInterval\":" + std::to_string(m_actualState.getActualPollInterval()) + "}";

    actual_state_topic.publish(std::move(mqtt_payload));
}

void DigitalTwin::publishActualStateDisplayText(std::string displayText)
{
    const int QOS = 0;
    mqtt::topic actual_state_topic(m_mqttClient, m_desiredStateConfiguration->getTopicString() + "/actualState", QOS, false);
    std::string mqtt_payload
        = "{\"dateString\": \"" + getDateTimeString() + "\", \"displayText\": \"" + displayText + "\"}";

    actual_state_topic.publish(std::move(mqtt_payload));
}

void DigitalTwin::publishVcc(std::string voltage)
{
    const int QOS = 0;

    mqtt::topic json_topic(m_mqttClient, createMqttTopic("NDATA", m_name, ""), QOS, false);

    try {
        std::string mqtt_payload
            = "{\"dateString\": \"" + getDateTimeString() + "\", \"voltage\":" + voltage + "}";

        json_topic.publish(std::move(mqtt_payload));
    }
    catch (const mqtt::exception& exc) {
        std::cerr << exc.what() << std::endl;
    }
}

void DigitalTwin::publishNdeath()
{
    const int QOS = 0;

    mqtt::topic slave_death(
        m_mqttClient, createMqttTopic("NDEATH", m_name, ""), QOS, false);
    slave_death.publish(std::move(getDateTimeString()));
}

