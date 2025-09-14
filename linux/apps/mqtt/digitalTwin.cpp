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

void DigitalTwin::execute()
{
    if (m_desiredState->getDesiredPollInterval() != m_actualState.getActualPollInterval()) {
        m_actualState.setActualPollInterval(m_desiredState->getDesiredPollInterval());
        publishDesiredStatePollInterval();
    }

    if ((secondsSinceEpoch() - m_timeLastPoll) > m_actualState.getActualPollInterval()) {
        if (m_radioSession.wakeupNotResponding()) {
            if(readVccAndPublish())
            {
                m_timeLastPoll = secondsSinceEpoch();
            }
            readGpioAndPublish();
        }
    }

    if (m_desiredState->getDesiredDisplayText() != m_actualState.getActualDisplayText()) {
        if (m_radioSession.wakeupNotResponding()) {
            m_radioSession.setKeepAliveInterval(50);
            updateDisplayText();
            m_actualState.setActualDisplayText(m_desiredState->getDesiredDisplayText());
        }
    }

    //m_radioSession.close();
}

bool DigitalTwin::readVccAndPublish()
{
    bool retval = false;
    auto nodeVcc = m_monitor.getRadio<>(UartCommandVcc());

    if (nodeVcc.getReplyStatus() == UartCommandBase::ReplyStatus::Complete) {
        uint16_t vcc_mv = nodeVcc.responseStruct().getVcc();
        publishVcc(std::to_string(vcc_mv / 1000.0));
        retval = true;
    }
    else {
        // TODO: publish this after n failed read attempts
        publishNdeath();
    }

    return(retval);
}

void DigitalTwin::readGpioAndPublish()
{
    auto nodeGpio = m_monitor.getRadio<>(UartCommandGpio());

    if (nodeGpio.getReplyStatus() == UartCommandBase::ReplyStatus::Complete) {
        uint8_t portB = nodeGpio.responseStruct().portB;
        uint8_t portC = nodeGpio.responseStruct().portC;
        uint8_t portD = nodeGpio.responseStruct().portD;

        publishGpio(portB, portC, portD);
    }
    else {
        // TODO: publish this after n failed read attempts
        publishNdeath();
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
    mqtt::topic actual_state_topic(m_mqttClient, m_desiredState->getTopicString() + "/actualState", QOS, false);
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

void DigitalTwin::publishGpio(uint8_t portB, uint8_t portC, uint8_t portD)
{
    const int QOS = 0;

    std::string portBstring = std::to_string((int) portB);
    std::string portCstring = std::to_string((int) portC);
    std::string portDstring = std::to_string((int) portD);

    mqtt::topic json_topic(m_mqttClient, createMqttTopic("NDATA", m_name, "gpio"), QOS, false);

    try {
        std::string mqtt_payload = "{\"dateString\": \"" + getDateTimeString() + "\", \"portB\": \"" + portBstring + "\", \"portC\": \"" + portCstring + "\", \"portD\": \"" + portDstring + "\" }";

        json_topic.publish(std::move(mqtt_payload));
    }
    catch (const mqtt::exception& exc) {
        std::cerr << exc.what() << std::endl;
    }
}

void DigitalTwin::publishNdeath()
{
    const int QOS = 0;

    mqtt::topic nodeDeath(
        m_mqttClient, createMqttTopic("NDEATH", m_name, ""), QOS, false);
    nodeDeath.publish(std::move(getDateTimeString()));
}

