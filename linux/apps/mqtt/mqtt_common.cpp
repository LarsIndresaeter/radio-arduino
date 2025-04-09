#include "mqtt_common.hpp"

using namespace std::chrono_literals;
using time_point = std::chrono::system_clock::time_point;

using nlohmann::json;

uint64_t secondsSinceEpoch()
{
    using namespace std::chrono;
    uint64_t seconds
        = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())
                  .count())
        / 1000;
    // std::cout << "time " << std::to_string(seconds) << std::endl;
    return seconds;
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

void publishNbirth(mqtt::async_client& mqtt_client, std::string nodeName)
{
    mqtt::topic master_birth(
        mqtt_client, createMqttTopic("NBIRTH", nodeName, ""), 0, false);
    master_birth.publish(std::move("{\"dateString: \"" + getDateTimeString() + "\"}"));
}

void publishMonitorProtocolStatistics(monitor& mon, mqtt::async_client& mqtt_client, std::string& masterName)
{
    if (!masterName.empty()) {
        int QOS = 0;

        mqtt::topic link_commands_sent_topic(
            mqtt_client, createMqttTopic("NDATA", masterName, "protocol/commands_sent"), QOS, false);
        mqtt::topic link_valid_responses_topic(
            mqtt_client, createMqttTopic("NDATA", masterName, "protocol/valid_responses"), QOS, false);
        mqtt::topic link_invalid_responses_topic(
            mqtt_client, createMqttTopic("NDATA", masterName, "protocol/invalid_responses"), QOS, false);
        mqtt::topic link_bytes_sent_topic(
            mqtt_client, createMqttTopic("NDATA", masterName, "protocol/bytes_sent"), QOS, false);
        mqtt::topic link_bytes_received_topic(
            mqtt_client, createMqttTopic("NDATA", masterName, "protocol/bytes_received"), QOS, false);

        link_commands_sent_topic.publish(std::move(std::to_string(mon.getCommandsSent())));
        link_valid_responses_topic.publish(std::move(std::to_string(mon.getValidResponses())));
        link_invalid_responses_topic.publish(std::move(std::to_string(mon.getInvalidResponses())));
        link_bytes_sent_topic.publish(std::move(std::to_string(mon.getBytesSent())));
        link_bytes_received_topic.publish(std::move(std::to_string(mon.getBytesReceived())));
    }
}

void publishDesiredStatePollInterval(mqtt::async_client& mqtt_client, std::shared_ptr<DesiredStateConfiguration> dsc)
{
    const int QOS = 0;

    mqtt::topic actual_state_topic(mqtt_client, createMqttTopic("STATE", dsc->getName(), "actualState"), QOS, false);
    std::string mqtt_payload
        = "{\"dateString\": \"" + getDateTimeString() + "\", \"pollInterval\":" + std::to_string(dsc->getActualPollInterval()) + "}";

    actual_state_topic.publish(std::move(mqtt_payload));
}

void publishActualStateDisplayText(mqtt::async_client& mqtt_client, std::string topic, std::string displayText)
{
    const int QOS = 0;
    mqtt::topic actual_state_topic(mqtt_client, topic + "/actualState", QOS, false);
    std::string mqtt_payload
        = "{\"dateString\": \"" + getDateTimeString() + "\", \"displayText\": \"" + displayText + "\"}";

    actual_state_topic.publish(std::move(mqtt_payload));
}

void publishNdeath(mqtt::async_client& mqtt_client, std::string slaveName)
{
    const int QOS = 0;

    mqtt::topic slave_death(
        mqtt_client, createMqttTopic("NDEATH", slaveName, ""), QOS, false);
    slave_death.publish(std::move(getDateTimeString()));
}

void publishVcc(mqtt::async_client& mqtt_client, std::string slaveName, std::string voltage)
{
    const int QOS = 0;

    mqtt::topic json_topic(mqtt_client, createMqttTopic("NDATA", slaveName, ""), QOS, false);


    try {
        std::string mqtt_payload
            = "{\"dateString\": \"" + getDateTimeString() + "\", \"voltage\":" + voltage + "}";

        json_topic.publish(std::move(mqtt_payload));
    }
    catch (const mqtt::exception& exc) {
        std::cerr << exc.what() << std::endl;
    }
}



void updateDisplayText(monitor& mon, mqtt::async_client& mqtt_client, std::shared_ptr<DesiredStateConfiguration> dsc)
{
    //std::cout << "DEBUG: updateDisplayText, nodeName=" << nodeName << std::endl;

    std::vector<uint8_t> lcd(COMMANDS::SSD1306::STRING_LENGTH, ' ');
    std::string displayText = dsc->getDesiredDisplayText();

    //std::cout << "DEBUG: updateDisplayText, displayText=" << displayText << std::endl;

    for (uint8_t i = 0; i < displayText.size() && i < COMMANDS::SSD1306::STRING_LENGTH; i++) {
        lcd.at(i) = displayText.at(i);
    }

    auto response = mon.getRadio<>(UartCommandSsd1306(2, lcd), static_cast<std::chrono::milliseconds>(500));
    if (response.getReplyStatus() == UartCommandBase::ReplyStatus::Complete) {
        dsc->setActualDisplayText();
        publishActualStateDisplayText(mqtt_client, dsc->getTopicString(), displayText);
    }
}



std::string getSlaveNameAndPublishBirth(monitor& mon, mqtt::async_client& mqtt_client)
{
    std::string slaveName("");

    auto slaveDeviceInfo = mon.getRadio<>(UartCommandGetDeviceInfo());

    if (slaveDeviceInfo.getReplyStatus() != UartCommandBase::ReplyStatus::Complete) {
        slaveDeviceInfo = mon.getRadio<>(UartCommandGetDeviceInfo());
    }

    if (slaveDeviceInfo.getReplyStatus() == UartCommandBase::ReplyStatus::Complete) {
        auto response = slaveDeviceInfo.responseStruct();

        for (int i = 0; i < 16 && response.name[i] != 0; i++) {
            slaveName += response.name[i];
        }

        publishNbirth(mqtt_client, slaveName);
    }

    return (slaveName);
}

std::string getMasterNameAndPublishBirth(monitor& mon, mqtt::async_client& mqtt_client)
{
    std::string masterName("");

    auto masterDeviceInfo = mon.get<>(UartCommandGetDeviceInfo());

    if (masterDeviceInfo.getReplyStatus() != UartCommandBase::ReplyStatus::Complete) {
        masterDeviceInfo = mon.get<>(UartCommandGetDeviceInfo());
    }

    if (masterDeviceInfo.getReplyStatus() == UartCommandBase::ReplyStatus::Complete) {
        auto response = masterDeviceInfo.responseStruct();

        for (int i = 0; i < 16 && response.name[i] != 0; i++) {
            masterName += response.name[i];
        }

        publishNbirth(mqtt_client, masterName);
    }

    return (masterName);
}


