#include "digitalTwin.hpp"

DigitalTwin::DigitalTwin(monitor& monitor, mqtt::async_client& mqtt_client, uint8_t radioAddress, std::string name)
    : m_radioAddress(radioAddress)
    , m_monitor(monitor)
    , m_radioSession(monitor, radioAddress)
    , m_desiredStateConfiguration(radioAddress, name)
    , m_mqttClient(mqtt_client)
{
}

DesiredStateConfiguration DigitalTwin::getDesiredStateConfiguration()
{
    return(m_desiredStateConfiguration);
}

void DigitalTwin::execute()
{
    //std::cout << "DEBUG: DigitalTwin: address=" + std::to_string(m_radioAddress) << std::endl;

    if (m_desiredStateConfiguration.getDesiredPollInterval() != m_desiredStateConfiguration.getActualPollInterval()) {
        m_desiredStateConfiguration.setActualPollInterval(m_desiredStateConfiguration.getDesiredPollInterval());
        publishDesiredStatePollInterval(m_mqttClient, std::make_shared<DesiredStateConfiguration>(m_desiredStateConfiguration));
    }

    if ((secondsSinceEpoch() - m_desiredStateConfiguration.getTimeLastPoll()) > m_desiredStateConfiguration.getActualPollInterval()) {
        if(m_radioSession.wakeupNotResponding())
        {
            m_desiredStateConfiguration.setTimeLastPoll(secondsSinceEpoch());
            std::string slaveName = m_desiredStateConfiguration.getName();
            readVccAndPublish(m_monitor, m_mqttClient, slaveName); // TODO: return true if success
        }
    }

    if (m_desiredStateConfiguration.displayTextChanged()) {
        if(m_radioSession.wakeupNotResponding())
        {
            m_radioSession.setKeepAliveInterval(50);
            updateDisplayText(m_monitor, m_mqttClient, std::make_shared<DesiredStateConfiguration>(m_desiredStateConfiguration));
        }
    }


}
