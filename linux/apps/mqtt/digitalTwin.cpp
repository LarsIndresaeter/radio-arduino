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
        publishDesiredStatePollInterval();
    }
}

void DigitalTwin::execute()
{
    reconsileState();

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
            updateDisplayText();
            m_actualState.setActualDisplayText(m_desiredState->getDesiredDisplayText());
        }
    }

    //m_radioSession.close();
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

bool DigitalTwin::readVccAndPublish()
{
    bool retval = false;
    auto nodeVcc = m_monitor.getRadio<>(RaduinoCommandVcc());

    if (m_monitor.lastCommandReturnedValidResponse()) {
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
    auto nodeGpio = m_monitor.getRadio<>(RaduinoCommandGpio());

    if (m_monitor.lastCommandReturnedValidResponse()) {
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

    auto response = m_monitor.getRadio<>(RaduinoCommandSsd1306(2, lcd), static_cast<std::chrono::milliseconds>(500));
    if (m_monitor.lastCommandReturnedValidResponse()) {
        publishActualStateDisplayText(displayText);
    }
}

void DigitalTwin::publishDesiredStatePollInterval()
{
    std::string topic = createMqttTopic("STATE", m_name, "actualState");
    std::string message
        = "{\"dateString\": \"" + getDateTimeString() + "\", \"pollInterval\":" + std::to_string(m_actualState.getActualPollInterval()) + "}";

    publishMessage(topic, message);
}

void DigitalTwin::publishActualStateDisplayText(std::string displayText)
{
    std::string topic  = m_desiredState->getTopicString() + "/actualState";
    std::string message
        = "{\"dateString\": \"" + getDateTimeString() + "\", \"displayText\": \"" + displayText + "\"}";

    publishMessage(topic, message);
}

void DigitalTwin::publishVcc(std::string voltage)
{
    std::string topic = createMqttTopic("NDATA", m_name, "");

    std::string message
        = "{\"dateString\": \"" + getDateTimeString() + "\", \"voltage\":" + voltage + "}";

    publishMessage(topic, message);
}

void DigitalTwin::publishGpio(uint8_t portB, uint8_t portC, uint8_t portD)
{

    std::string portBstring = std::to_string((int) portB);
    std::string portCstring = std::to_string((int) portC);
    std::string portDstring = std::to_string((int) portD);

    std::string topic = createMqttTopic("NDATA", m_name, "gpio");

    std::string message = "{\"dateString\": \"" + getDateTimeString() + "\", \"portB\": \"" + portBstring + "\", \"portC\": \"" + portCstring + "\", \"portD\": \"" + portDstring + "\" }";

    publishMessage(topic, message);
}

void DigitalTwin::publishNdeath()
{
    std::string topic = createMqttTopic("NDEATH", m_name, "");
    std::string message = getDateTimeString();
    publishMessage(topic, message);
}

