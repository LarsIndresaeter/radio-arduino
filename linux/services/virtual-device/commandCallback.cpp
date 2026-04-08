#include "commandCallback.hpp"
#include "include/commandCallback.hpp"

using nlohmann::json;

CommandCallback::CommandCallback(

    mqtt::async_client& mqtt_client)
    : m_mqttClient(mqtt_client)
{
}
void CommandCallback::publishMessage(std::string topic, std::string message)
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

std::vector<std::string> CommandCallback::splitString(std::string s, const std::string& delimiter)
{
    std::vector<std::string> tokens;
    size_t pos = 0;
    std::string token;

    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        tokens.push_back(token);
        s.erase(0, pos + delimiter.length());
    }

    tokens.push_back(s);
    return tokens;
}

void CommandCallback::message_arrived(mqtt::const_message_ptr message)
{
    std::string topic_orig = message->get_topic();
    std::string payload = message->get_payload_str();
    if (topic_orig.starts_with("raduino-subscription/DDATA/")) {
        try {
            // std::cout << "DEBUG: payload:" << payload << std::endl;
            auto jsonData = json::parse(payload);
            std::string commandName = jsonData["name"];
            uint64_t timestamp = jsonData["timestamp"].get<uint64_t>();

            // std::cout << "DEBUG: name:" << commandName << ", timestamp:" << std::to_string(timestamp) << std::endl;

            auto tokens = splitString(topic_orig, "/");
            std::string nodeAddressString = tokens.at(2);

            devices[nodeAddressString]["timestamp"] = timestamp;

            if (commandName == "ina219") {
                int16_t voltage = jsonData["payload"]["voltage"].get<uint16_t>();
                int16_t current = jsonData["payload"]["current"].get<uint16_t>();
                devices[nodeAddressString]["ina219"]["voltage"] = voltage*0.001;
                devices[nodeAddressString]["ina219"]["current"] = current*0.004;
            }

            if (commandName == "gpio") {
                devices[nodeAddressString]["gpio"]["portB"] = jsonData["payload"]["portB"];
                devices[nodeAddressString]["gpio"]["portC"] = jsonData["payload"]["portC"];
                devices[nodeAddressString]["gpio"]["portD"] = jsonData["payload"]["portD"];
            }

            if (commandName == "vcc") {
                uint16_t vcc = jsonData["payload"]["vcc"];
                devices[nodeAddressString]["batteryVoltage"] = (vcc*1.0)/1000;
            }

            if (commandName == "ds18b20") {
                uint16_t temperature = jsonData["payload"]["temperature"];
                devices[nodeAddressString]["temperature"] = (temperature*1.0)/16;
            }

            if (commandName == "get_device_name") {
                devices[nodeAddressString]["deviceName"] = jsonData["payload"]["nameString"];
            }

            if (commandName == "get_version") {
                devices[nodeAddressString]["version"] = jsonData["payload"]["versionString"];
            }

            if (commandName == "quadrature_encoder") {
                int countnegative = jsonData["payload"]["countnegative"];
                int countpositive = jsonData["payload"]["countpositive"];
                devices[nodeAddressString]["quadrature_encoder"] = countpositive - countnegative;
            }

            if (commandName == "get_lsm303d") {
                int x = jsonData["payload"]["accelerometerX"].get<uint16_t>();
                int y = jsonData["payload"]["accelerometerY"].get<uint16_t>();
                int z = jsonData["payload"]["accelerometerZ"].get<uint16_t>();
                devices[nodeAddressString]["accelerometer"]["x"] = (180*(x - 32768))/32768;
                devices[nodeAddressString]["accelerometer"]["y"] = (180*(y - 32768))/32768;
                devices[nodeAddressString]["accelerometer"]["z"] = (180*(z - 32768))/32768;
            }

            std::string topic = "raduino-device/status";

            publishMessage(topic, devices.dump());
        }
        catch (std::exception const& e) {
            std::cout << "ERROR: malformed DDATA" << std::endl;
        }
    }
}

void CommandCallback::connection_lost(const std::string& cause)
{
    std::cout << "Connection lost: " << cause << std::endl;
}

void CommandCallback::delivery_complete(mqtt::delivery_token_ptr token)
{
    std::cout << "Message delivered" << std::endl;
}

