#include "deviceController.hpp"
#include "include/deviceController.hpp"
#include <string>

using nlohmann::json;

const char* TOPIC_BROADCAST = "DBIRTH";
const char* TOPIC_ADVERTISEMENT = "ADVERTISEMENT";
const char* TOPIC_COMMAND_PREFIX = "raduino-adapter/RCMD/command/";
const char* TOPIC_RESPONSE_PREFIX = "DDATA";

DeviceController::DeviceController(
    monitor& monitor, mqtt::async_client& mqtt_client, uint32_t radioAddress, uint32_t gatewayAddress)
    : m_radioAddress(radioAddress)
    , m_monitor(monitor)
    , m_radioSession(monitor, radioAddress)
    , m_mqttClient(mqtt_client)
    , m_gatewayAddress(gatewayAddress)
{
    m_topic = createMqttTopic("RCMD", std::to_string(m_gatewayAddress), std::to_string(m_radioAddress));

    if (radioAddress == gatewayAddress) {
        m_nodeType = "gateway";
    }
    else {
        m_nodeType = "node";
    }
}

uint32_t DeviceController::getNodeAddress() { return (m_radioAddress); }

void DeviceController::publishBirth()
{
    std::string topic
        = createMqttTopic(TOPIC_BROADCAST, std::to_string(m_gatewayAddress), std::to_string(m_radioAddress));

    json birthCertificate = { { "nodeAddress", m_radioAddress },
                              { "gateway", m_gatewayAddress },
                              { "healthIndicator", healthIndicator },
                              { "lastAdvertisement", timestampLastDiscovery },
                              { "validResponses", m_validJsonResponses } };

    publishMessage(topic, birthCertificate.dump());
    setPublishBirth(false);
}

void DeviceController::execute()
{
    if (m_commandReceived) {
        executeJsonCommand();

        m_command = "";
        m_commandReceived = false;
        publishState();
    }

    if (m_publishBirth) {
        publishBirth();
    }

    if (m_publishAdvertisement) {
        std::string topic
            = createMqttTopic(TOPIC_ADVERTISEMENT, std::to_string(m_gatewayAddress), std::to_string(m_radioAddress));

        json advertisement = { { "nodeAddress", m_radioAddress },
                               { "lastAdvertisement", timestampLastDiscovery },
                               { "nodeType", m_nodeType } };

        publishMessage(topic, advertisement.dump());
        setPublishAdvertisement(false);
    }

    m_lastDeviceIdSeen = m_monitor.get<>(RaduinoCommandGetLastDeviceIdSeen()).responseStruct().getId();
}

void DeviceController::publishState()
{
    std::string topic
        = createMqttTopic("STATE", std::to_string(m_gatewayAddress) + "/" + std::to_string(m_radioAddress), "command");
    publishMessage(topic, m_command);
}

void DeviceController::discoveryReceived(uint32_t nodeAddress)
{
    using namespace std::chrono;
    if (nodeAddress == m_radioAddress) {
        uint64_t timestamp = (duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count());

        setPublishAdvertisement(true);

        timestampLastDiscovery = timestamp;

        if (healthIndicator < 0) {
            healthIndicator = 0; // discovery received but no response yet
            setPublishBirth(true);
        }
        else {
            healthIndicator++;
        }
    }
}

uint32_t DeviceController::getLastDeviceIdSeen() { return (m_lastDeviceIdSeen); }

void DeviceController::log(std::string type, std::string message)
{
    std::string topic = "raduino-log/adapter/" + std::to_string(m_radioAddress) + "/" + type;
    json jsonMessage = { "message", message };
    publishMessage(topic, jsonMessage.dump());
}

void DeviceController::logError(std::string message) { log("error", message); }
void DeviceController::logWarning(std::string message) { log("warning", message); }
void DeviceController::logDebug(std::string message) { log("debug", message); }

void DeviceController::executeJsonCommand()
{
    try {
        auto jsonData = json::parse(m_command);

        uint32_t nodeAddress = jsonData["nodeAddress"].get<uint32_t>();
        bool radioNodeWakeupSucces = false;

        if (nodeAddress == m_radioAddress) {
            if (nodeAddress != m_gatewayAddress) {
                if (m_radioSession.wakeupNotResponding()) {
                    radioNodeWakeupSucces = true;
                }
                else {
                    logError("not able to wake up node");

                    healthIndicator = 0;
                    m_publishBirth = true;
                }
            }

            json commandList = jsonData["commandList"];

            for (std::string commandName : commandList) {
                std::string jsonResponse = "";

                if (nodeAddress == m_gatewayAddress) {
                    if (commandName == "get_version") {
                        jsonResponse = m_monitor.get<>(RaduinoCommandGetVersion()).getJson();
                    }
                    else if (commandName == "get_device_name") {
                        jsonResponse = m_monitor.get<>(RaduinoCommandGetDeviceName()).getJson();
                    }
                    else if (commandName == "get_statistics") {
                        jsonResponse = m_monitor.get<>(RaduinoCommandGetStatistics()).getJson();
                    }
                    else if (commandName == "get_attached_devices_csv_string") {
                        jsonResponse = m_monitor.get<>(RaduinoCommandGetAttachedDevicesCsvString()).getJson();
                    }
                    else if (commandName == "ina219") {
                        jsonResponse = m_monitor.get<>(RaduinoCommandIna219()).getJson();
                    }
                    else {
                        logDebug(commandName = " not recognized for gateway");
                    }
                }
                else {
                    if (commandName == "vcc") {
                        jsonResponse = m_monitor.getRadio<>(RaduinoCommandVcc()).getJson();
                    }
                    else if (commandName == "get_version") {
                        jsonResponse = m_monitor.getRadio<>(RaduinoCommandGetVersion(), 1000ms).getJson();
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
                        jsonResponse
                            = m_monitor.getRadio<>(RaduinoCommandGetAttachedDevicesCsvString(), 1000ms).getJson();
                    }
                    else if (commandName == "get_active_time_counter") {
                        using namespace std::chrono;
                        auto start = std::chrono::high_resolution_clock::now();
                        uint64_t time_since_epoch_ms
                            = std::chrono::duration_cast<std::chrono::milliseconds>(start.time_since_epoch()).count();

                        std::vector<uint8_t> timestamp;
                        timestamp.push_back(time_since_epoch_ms);
                        timestamp.push_back(time_since_epoch_ms >> 8);
                        timestamp.push_back(time_since_epoch_ms >> 16);
                        timestamp.push_back(time_since_epoch_ms >> 24);
                        timestamp.push_back(time_since_epoch_ms >> 32);
                        timestamp.push_back(time_since_epoch_ms >> 40);
                        timestamp.push_back(time_since_epoch_ms >> 48);
                        timestamp.push_back(time_since_epoch_ms >> 56);

                        jsonResponse = m_monitor.getRadio<>(RaduinoCommandGetActiveTimeCounter(timestamp)).getJson();
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
                    else if (commandName == "ina219") {
                        jsonResponse = m_monitor.getRadio<>(RaduinoCommandIna219()).getJson();
                    }
                    else {
                        logDebug(commandName + " not recognized for radio node");
                    }
                }

                if (jsonResponse != "") {
                    std::string topic = createMqttTopic(
                        TOPIC_RESPONSE_PREFIX,
                        std::to_string(m_gatewayAddress) + "/" + std::to_string(m_radioAddress),
                        commandName);

                    if (m_monitor.lastCommandReturnedValidResponse()) {
                        storeValidResponse(jsonResponse);
                    }

                    publishMessage(topic, jsonResponse);
                }

                updateQualityIndicator(m_monitor.lastCommandReturnedValidResponse());
            }

            if (radioNodeWakeupSucces) {
                m_monitor.getRadio<>(RaduinoCommandKeepAlive(0)); // put radio node in sleep
            }
        }
    }
    catch (std::exception const& e) {
        std::cerr << e.what() << std::endl;
    }
}

void DeviceController::storeValidResponse(std::string jsonResponse)
{
    try {
        auto parsedJsonResponse = json::parse(jsonResponse);
        std::string responseCode = parsedJsonResponse["responseCode"];
        std::string name = parsedJsonResponse["name"];
        if (responseCode == "success") {
            m_validJsonResponses[name] = parsedJsonResponse;
        }
    }
    catch (std::exception const& e) {
        std::cerr << e.what() << std::endl;
    }
}

void DeviceController::updateQualityIndicator(bool successfulResponse)
{
    if (successfulResponse) {
        if (healthIndicator < 1) {
            healthIndicator = 1;
        }
        else {
            healthIndicator += 1000; // count consequitive successful responses
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
    catch (std::exception const& e) {
        std::cerr << e.what() << std::endl;
        std::cerr << "topic: " << topic << std::endl;
        std::cerr << "message: " << message << std::endl;
    }
}

std::string DeviceController::createMqttTopic(std::string type, std::string eon, std::string device)
{
    std::string retval("raduino-adapter/" + type + "/" + eon);
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
            else {
                // std::cout << "WARNING: Ignore command, reason: busy parsing previous command. nodeAddress="
                //<< std::to_string(m_radioAddress) << std::endl;
                healthIndicator = 0;
                setPublishBirth(true);
            }
        }
    }
    catch (std::exception const& e) {
        std::cerr << e.what() << std::endl;
    }
}

void DeviceController::setPublishBirth(bool value) { m_publishBirth = value; }

void DeviceController::setPublishAdvertisement(bool value) { m_publishAdvertisement = value; }

std::string DeviceController::getTopicString() { return (m_topic); }

