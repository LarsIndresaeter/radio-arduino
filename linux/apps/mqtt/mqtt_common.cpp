#include "mqtt_common.hpp"

using namespace std::chrono_literals;
using time_point = std::chrono::system_clock::time_point;

using nlohmann::json;

uint64_t milliSecondsSinceEpoch()
{
    using namespace std::chrono;
    uint64_t seconds
        = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())
                .count());
    return seconds;
}

uint64_t secondsSinceEpoch()
{
    return milliSecondsSinceEpoch()/1000;
}

std::string createMqttTopic(std::string type, std::string eon, std::string device)
{
    std::string retval("radio-arduino/" + type + "/" + eon);
    if (!device.empty()) {
        retval.append("/" + device);
    }
    return (retval);
}

std::string serializeTimePoint(const time_point& time, const std::string& format)
{
    std::time_t tt = std::chrono::system_clock::to_time_t(time);
    std::tm tm = *std::gmtime(&tt); // GMT (UTC)
    // std::tm tm = *std::localtime(&tt); //Locale time-zone, usually UTC by default.
    std::stringstream ss;
    ss << std::put_time(&tm, format.c_str());
    return ss.str();
}

std::string getDateTimeString()
{
    time_point input = std::chrono::system_clock::now();
    std::string dateString = serializeTimePoint(input, "%Y-%m-%d %H:%M:%S");
    return (dateString);
}

void publishMonitorProtocolStatistics(monitor& mon, mqtt::async_client& mqtt_client, std::string& gatewayName)
{
    if (!gatewayName.empty()) {
        int QOS = 0;

        mqtt::topic link_commands_sent_topic(
            mqtt_client, createMqttTopic("NDATA", gatewayName, "protocol/commands_sent"), QOS, false);
        mqtt::topic link_valid_responses_topic(
            mqtt_client, createMqttTopic("NDATA", gatewayName, "protocol/valid_responses"), QOS, false);
        mqtt::topic link_invalid_responses_topic(
            mqtt_client, createMqttTopic("NDATA", gatewayName, "protocol/invalid_responses"), QOS, false);
        mqtt::topic link_bytes_sent_topic(
            mqtt_client, createMqttTopic("NDATA", gatewayName, "protocol/bytes_sent"), QOS, false);
        mqtt::topic link_bytes_received_topic(
            mqtt_client, createMqttTopic("NDATA", gatewayName, "protocol/bytes_received"), QOS, false);

        link_commands_sent_topic.publish(std::move(std::to_string(mon.getCommandsSent())));
        link_valid_responses_topic.publish(std::move(std::to_string(mon.getValidResponses())));
        link_invalid_responses_topic.publish(std::move(std::to_string(mon.getInvalidResponses())));
        link_bytes_sent_topic.publish(std::move(std::to_string(mon.getBytesSent())));
        link_bytes_received_topic.publish(std::move(std::to_string(mon.getBytesReceived())));
    }
}

void publishNbirth(mqtt::async_client& mqtt_client, std::string deviceName)
{
    mqtt::topic deviceBirth(
        mqtt_client, createMqttTopic("NBIRTH", deviceName, ""), 0, false);
    deviceBirth.publish(std::move("{\"dateString: \"" + getDateTimeString() + "\"}"));
}

std::string getNodeNameAndPublishBirth(monitor& mon, mqtt::async_client& mqtt_client)
{
    std::string nodeName("");

    auto nodeDeviceInfo = mon.getRadio<>(RaduinoCommandGetDeviceName());

    if (mon.lastCommandReturnedValidResponse()) {
        nodeDeviceInfo = mon.getRadio<>(RaduinoCommandGetDeviceName());
    }

    if (mon.lastCommandReturnedValidResponse()) {
        auto response = nodeDeviceInfo.responseStruct();

        for (int i = 0; i < sizeof(response.nameString) && response.nameString[i] != 0; i++) {
            nodeName += response.nameString[i];
        }

        publishNbirth(mqtt_client, nodeName);
    }

    return (nodeName);
}

std::string getGatewayNameAndPublishBirth(monitor& mon, mqtt::async_client& mqtt_client)
{
    std::string gatewayName("");

    auto nodeDeviceInfo = mon.get<>(RaduinoCommandGetDeviceName());

    if (mon.lastCommandReturnedValidResponse()) {
        nodeDeviceInfo = mon.get<>(RaduinoCommandGetDeviceName());
    }

    if (mon.lastCommandReturnedValidResponse()) {
        auto response = nodeDeviceInfo.responseStruct();

        for (int i = 0; i < sizeof(response.nameString) && response.nameString[i] != 0; i++) {
            gatewayName += response.nameString[i];
        }

        publishNbirth(mqtt_client, gatewayName);
    }

    return (gatewayName);
}

