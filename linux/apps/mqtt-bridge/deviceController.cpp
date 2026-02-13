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
    , m_mqttClient(mqtt_client)
    , m_nodeName(nodeName)
    , m_gatewayName(gatewayName)
{
    m_topic = createMqttTopic("RCMD", m_gatewayName, std::to_string(m_radioAddress));
}

uint32_t DeviceController::getNodeAddress() { return (m_radioAddress); }

void DeviceController::execute()
{
    if (m_commandReceived) {
        if (m_radioSession.wakeupNotResponding()) {
            executeJsonCommand();
            m_radioSession.close();
        }
        else {
            std::cout << "DEBUG: not able to wake up node" << std::endl;
            updateQualityIndicator(false);
        }

        setPublishBirth(true);

        m_command = "";
        m_commandReceived = false;
        publishState();
    }

    if (m_publishBirth) {
        std::string topic = createMqttTopic(TOPIC_BROADCAST, m_gatewayName, std::to_string(m_radioAddress));
        std::string message = "{\"nodeAddress\": " + std::to_string(m_radioAddress) + ", \"gateway\": \"" + m_gatewayName
            + "\"" + ", \"healthIndicator\": " + std::to_string(healthIndicator)
            + ", \"lastSeen\": " + std::to_string(timestampLastDiscovery) + "}";
        publishMessage(topic, message);
        setPublishBirth(false);
    }
}

void DeviceController::publishState()
{
    std::string topic = createMqttTopic("STATE", m_gatewayName + "/" + std::to_string(m_radioAddress), "command");
    publishMessage(topic, m_command);
}

void DeviceController::discoveryReceived(uint32_t nodeAddress)
{
    using namespace std::chrono;
    if (nodeAddress == m_radioAddress) {
        uint64_t timestamp = (duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count());

        if ((timestamp - timestampLastDiscovery) > 60000) {
            setPublishBirth(true);
        }

        timestampLastDiscovery = timestamp;

        if (healthIndicator < 0) {
            healthIndicator = 0; // discovery received but no response yet
            setPublishBirth(true);
        }
    }
}

void DeviceController::executeJsonCommand()
{
    try {
        auto jsonData = json::parse(m_command);

        uint32_t nodeAddress = jsonData.value("nodeAddress", 0);

        if (nodeAddress == m_radioAddress) {
            std::string commandName = jsonData["name"];

            std::string topic = createMqttTopic(
                TOPIC_RESPONSE_PREFIX, m_gatewayName + "/" + std::to_string(m_radioAddress), "response");

            std::string jsonResponse = "";

            m_radioSession.wakeupNotResponding();

            if (commandName == "vcc") {
                jsonResponse = m_monitor.getRadio<>(RaduinoCommandVcc()).getJson();
            }
            else if (commandName == "get_version") {
                jsonResponse = m_monitor.getRadio<>(RaduinoCommandGetVersion()).getJson();
            }
            else if (commandName == "get_device_name") {
                jsonResponse = m_monitor.getRadio<>(RaduinoCommandGetDeviceName()).getJson();
            }
            else if (commandName == "get_statistics") {
                jsonResponse = m_monitor.getRadio<>(RaduinoCommandGetStatistics()).getJson();
            }
            else if (commandName == "get_lsm303d") {
                jsonResponse = m_monitor.getRadio<>(RaduinoCommandGetLsm303d()).getJson();
            }
            else if (commandName == "get_attached_devices_csv_string") {
                jsonResponse = m_monitor.getRadio<>(RaduinoCommandGetAttachedDevicesCsvString()).getJson();
            }
            else if (commandName == "quadrature_encoder") {
                jsonResponse = m_monitor.getRadio<>(RaduinoCommandQuadratureEncoder()).getJson();
            }
            else if (commandName == "gpio") {
                jsonResponse = m_monitor.getRadio<>(RaduinoCommandGpio()).getJson();
            }
            else if (commandName == "ssd1306") {
                std::string displayText = jsonData["displayText"];
                jsonResponse = m_monitor.getRadio<>(RaduinoCommandSsd1306(2, displayText)).getJson();
            }

            if (jsonResponse != "") {
                publishMessage(topic, jsonResponse);
            }

            updateQualityIndicator(m_monitor.lastCommandReturnedValidResponse());
        }
    }
    catch (const mqtt::exception& exc) {
        std::cerr << exc.what() << std::endl;
    }
}

void DeviceController::updateQualityIndicator(bool successfulResponse)
{
    // seher
    // <0: number of consequitive errors
    //  0: got discovery message but no response or error yet
    // >0: number of consequitive responses
    if (successfulResponse) {
        if (healthIndicator < 1) {
            healthIndicator = 1;
        }
        else {
            healthIndicator++; // count consequitive successful responses
        }
    }
    else {
        if (healthIndicator > 0) {
            healthIndicator = -1;
        }
        else {
            healthIndicator--; // count consequitive missing responses
        }
    }
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

void DeviceController::parseMessage(std::string topic, std::string command)
{
    try {
        if (topic == m_topic) {
            if (!m_commandReceived) {
                m_command = command;
                m_commandReceived = true;
                publishState();
            }
        }
    }
    catch (const mqtt::exception& exc) {
        std::cerr << exc.what() << std::endl;
    }
}

void DeviceController::setPublishBirth(bool value) { m_publishBirth = value; }

std::string DeviceController::getTopicString() { return (m_topic); }

