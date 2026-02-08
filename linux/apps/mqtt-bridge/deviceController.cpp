#include "deviceController.hpp"
#include "include/deviceController.hpp"
#include <string>

using nlohmann::json;

const char* TOPIC_BROADCAST = "DBIRTH";
const char* TOPIC_COMMAND_PREFIX = "radio-arduino/RCMD/command/";
const char* TOPIC_RESPONSE_PREFIX = "DDATA";

DeviceController::DeviceController(
    monitor& monitor,
    mqtt::async_client& mqtt_client,
    uint32_t radioAddress,
    std::string nodeName,
    std::string gatewayName)
    : m_radioAddress(radioAddress)
    , m_monitor(monitor)
    , m_radioSession(monitor, radioAddress)
    , m_desiredState(std::make_shared<DesiredState>(radioAddress, nodeName, gatewayName))
    , m_mqttClient(mqtt_client)
    , m_nodeName(nodeName)
    , m_gatewayName(gatewayName)
{
}

std::shared_ptr<DesiredState> DeviceController::getDesiredState() { return (m_desiredState); }

uint32_t DeviceController::getNodeAddress() { return (m_radioAddress); }

void DeviceController::execute()
{
    if (m_desiredState->hasNewCommand()) {
        if (m_radioSession.wakeupNotResponding()) {
            try {
                executeJsonCommand(m_desiredState->getDesiredCommand());
            }
            catch (const mqtt::exception& exc) {
                std::cerr << exc.what() << std::endl;
            }
            m_radioSession.close();
        }
    }

    if (m_desiredState->publishBirth()) {
        std::string topic = createMqttTopic(TOPIC_BROADCAST, m_gatewayName, std::to_string(m_radioAddress));
        std::string message = "{\"address\": \"" + std::to_string(m_radioAddress) + "\", \"healthScore\": "
            + std::to_string(healthScore) + ", \"lastSeen\": " + std::to_string(timestampLastDiscovery) + "}";
        publishMessage(topic, message);
        m_desiredState->setPublishBirth(false);
    }
}

void DeviceController::discoveryReceived()
{
    using namespace std::chrono;
    uint64_t timestamp = (duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count());

    if ((timestamp - timestampLastDiscovery) > 60000) {
        m_desiredState->setPublishBirth(true);
    }

    timestampLastDiscovery = timestamp;
}

void DeviceController::executeJsonCommand(std::string command)
{
    auto jsonData = json::parse(command);
    std::string device = jsonData["device"];

    if (device == "node") {
        uint32_t nodeAddress = jsonData.value("nodeAddress", 0);

        m_radioSession.wakeupNotResponding();

        if (nodeAddress == m_radioAddress) {
            std::string commandName = jsonData["name"];
            // std::cout << "DEBUG: commandName='" << commandName << "'" << std::endl;

            if (commandName == "vcc") {
                std::cout << "DEBUG: publish vcc" << std::endl;

                auto response = m_monitor.getRadio<>(RaduinoCommandVcc());

                std::string topic = createMqttTopic(
                    TOPIC_RESPONSE_PREFIX,
                    m_gatewayName + "/" + std::to_string(m_radioAddress),
                    response.getCommandName());
                publishMessage(topic, response.getJson());
            }
            else if (commandName == "get_version") {
                auto response = m_monitor.getRadio<>(RaduinoCommandGetVersion());
                std::string topic = createMqttTopic(
                    TOPIC_RESPONSE_PREFIX,
                    m_gatewayName + "/" + std::to_string(m_radioAddress),
                    response.getCommandName());
                publishMessage(topic, response.getJson());
            }
            else if (commandName == "get_statistics") {
                auto response = m_monitor.getRadio<>(RaduinoCommandGetStatistics());
                std::string topic = createMqttTopic(
                    TOPIC_RESPONSE_PREFIX,
                    m_gatewayName + "/" + std::to_string(m_radioAddress),
                    response.getCommandName());
                publishMessage(topic, response.getJson());
            }
            else if (commandName == "get_lsm303d") {
                auto response = m_monitor.getRadio<>(RaduinoCommandGetLsm303d());
                std::string topic = createMqttTopic(
                    TOPIC_RESPONSE_PREFIX,
                    m_gatewayName + "/" + std::to_string(m_radioAddress),
                    response.getCommandName());
                publishMessage(topic, response.getJson());
            }
            else if (commandName == "get_attached_devices_csv_string") {
                auto response = m_monitor.getRadio<>(RaduinoCommandGetAttachedDevicesCsvString());
                std::string topic = createMqttTopic(
                    TOPIC_RESPONSE_PREFIX,
                    m_gatewayName + "/" + std::to_string(m_radioAddress),
                    response.getCommandName());
                publishMessage(topic, response.getJson());
            }
            else if (commandName == "quadrature_encoder") {
                auto response = m_monitor.getRadio<>(RaduinoCommandQuadratureEncoder());
                std::string topic = createMqttTopic(
                    TOPIC_RESPONSE_PREFIX,
                    m_gatewayName + "/" + std::to_string(m_radioAddress),
                    response.getCommandName());
                publishMessage(topic, response.getJson());
            }
            else if (commandName == "gpio") {
                auto response = m_monitor.getRadio<>(RaduinoCommandGpio());
                std::string topic = createMqttTopic(
                    TOPIC_RESPONSE_PREFIX,
                    m_gatewayName + "/" + std::to_string(m_radioAddress),
                    response.getCommandName());
                publishMessage(topic, response.getJson());
            }
            else if (commandName == "ssd1306") {
                std::string displayText = jsonData["displayText"];
                auto response = m_monitor.getRadio<>(RaduinoCommandSsd1306(2, displayText));
                std::string topic = createMqttTopic(
                    TOPIC_RESPONSE_PREFIX,
                    m_gatewayName + "/" + std::to_string(m_radioAddress),
                    response.getCommandName());
                publishMessage(topic, response.getJson());
            }

            if (m_monitor.lastCommandReturnedValidResponse()) {
                updateHealthScore(2);
            }
            else {
                updateHealthScore(0);
            }
        }
    }

    m_desiredState->clearDesiredCommand();
}

void DeviceController::updateHealthScore(int newHealthScore)
{
    /* 0: no connection
     * 1: present
     * 2: good times
     */

    if (newHealthScore != healthScore) {
        m_desiredState->setPublishBirth(true);
    }

    healthScore = newHealthScore;
}

void DeviceController::publishMessage(std::string topic, std::string message)
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

std::string DeviceController::createMqttTopic(std::string type, std::string eon, std::string device)
{
    std::string retval("radio-arduino/" + type + "/" + eon);
    if (!device.empty()) {
        retval.append("/" + device);
    }
    return (retval);
}

