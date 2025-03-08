
#include "mqtt/async_client.h"
#include <chrono>
#include <cmath>
#include <commands.hpp>
#include <eventprocess.hpp>
#include <filesystem>
#include <linuxCryptoHandler.hpp>
#include <monitor.hpp>
#include <numeric>
#include <string>
#include <thread>
#include <uart.hpp>

#include <chrono>
#include <iomanip>
#include <sstream>

#include <nlohmann/json.hpp>

using nlohmann::json;

using namespace std::chrono_literals;

constexpr float EVENT_THRESHOLD = 0.01; // 1% change before sending new message
constexpr uint EVENT_TIMEOUT = 10 * 60; // seconds before sending new message

using time_point = std::chrono::system_clock::time_point;
std::string serializeTimePoint(const time_point& time, const std::string& format)
{
    std::time_t tt = std::chrono::system_clock::to_time_t(time);
    std::tm tm = *std::gmtime(&tt); // GMT (UTC)
    // std::tm tm = *std::localtime(&tt); //Locale time-zone, usually UTC by default.
    std::stringstream ss;
    ss << std::put_time(&tm, format.c_str());
    return ss.str();
}

int timeMs()
{
    using namespace std::chrono;
    int ms
        = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())
                  .count())
        % 1000;
    return ms;
}

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

void print_usage()
{
    std::cout << "mqtt-client" << std::endl;
    std::cout << "       -N : ina219 power monitor, statistic per second for "
                 "<N> seconds"
              << std::endl;
    std::cout << "       -j : read voltage from rf slave" << std::endl;
    std::cout << "       -h : print this text" << std::endl;
}

std::string createMqttTopic(std::string type, std::string eon, std::string device)
{
    std::string retval("radio-arduino/" + type + "/" + eon);
    if (!device.empty()) {
        retval.append("/" + device);
    }
    return (retval);
}

float calculateStddev(std::vector<float> data)
{
    float average = std::reduce(data.begin(), data.end()) / data.size();
    float stddev = 0;

    for (std::vector<float>::const_iterator it = data.begin(); it != data.end();
         ++it) {
        float val = *it;
        stddev += pow(val - average, 2);
    }

    return stddev;
}

void readCurrentAndVoltage(monitor& mon, mqtt::async_client& mqtt_client, int samples)
{
    auto ina219 = mon.get<>(UartCommandIna219());
    std::vector<float> currentData, voltageData;

    const int QOS = 0;

    float I_avg_p = 0.0;
    float V_avg_p = 0.0;
    float I_std_p = 0.0;
    float V_min_p = 0.0;
    float V_max_p = 0.0;
    float I_min_p = 0.0;
    float I_max_p = 0.0;
    uint64_t timeLastMessage = 0;
    uint json_message_id = 0;
    bool timeoutExpired = true; // always send message on start up

    std::string masterName;

    for (int attempts = 0; attempts <= 10; attempts++) {
        auto deviceInfo = mon.get<>(UartCommandGetDeviceInfo());

        if (deviceInfo.getReplyStatus() == UartCommandBase::ReplyStatus::Complete) {
            try {
                auto response = deviceInfo.responseStruct();

                for (int i = 0; i < 16 && response.name[i] != 0; i++) {
                    masterName += response.name[i];
                }

                mqtt::topic device_name_topic(
                    mqtt_client, createMqttTopic("NBIRTH", masterName, ""), QOS, false);

                time_point input = std::chrono::system_clock::now();
                std::string dateString = serializeTimePoint(input, "%Y-%m-%d %H:%M:%S");

                device_name_topic.publish(dateString);
                break; // successfully read name and published it to the mqtt broker
            }
            catch (const mqtt::exception& exc) {
                std::cerr << exc.what() << std::endl;
            }
        }
    }

    mqtt::topic json_topic(mqtt_client, createMqttTopic("DDATA", masterName, "ina219"), QOS, false);
    mqtt::topic current_min_topic(
        mqtt_client, createMqttTopic("DDATA", masterName, "ina219/current/min"), QOS, false);
    mqtt::topic current_max_topic(
        mqtt_client, createMqttTopic("DDATA", masterName, "ina219/current/max"), QOS, false);
    mqtt::topic current_avg_topic(
        mqtt_client, createMqttTopic("DDATA", masterName, "ina219/current/avg"), QOS, false);
    mqtt::topic current_stddev_topic(
        mqtt_client, createMqttTopic("DDATA", masterName, "ina219/current/stddev"), QOS, false);
    mqtt::topic voltage_min_topic(
        mqtt_client, createMqttTopic("DDATA", masterName, "ina219/voltage/min"), QOS, false);
    mqtt::topic voltage_max_topic(
        mqtt_client, createMqttTopic("DDATA", masterName, "ina219/voltage/max"), QOS, false);
    mqtt::topic voltage_avg_topic(
        mqtt_client, createMqttTopic("DDATA", masterName, "ina219/voltage/avg"), QOS, false);

    for (int i = 0; i <= samples; i++) {
        int time = 1;
        int timePrev = 0;
        currentData.clear();
        voltageData.clear();
        // read data for 1 second
        while (time > timePrev) {
            ina219 = mon.get<>(UartCommandIna219());
            int16_t current_intval
                = (int16_t)(((uint16_t)ina219.responseStruct().current[0]) << 8
                    | ina219.responseStruct().current[1]);
            float current = current_intval * 0.001;
            currentData.push_back(current);

            int16_t voltage_intval
                = (int16_t)(((uint16_t)ina219.responseStruct().voltage[0]) << 8
                    | ina219.responseStruct().voltage[1]);
            voltage_intval = voltage_intval >> 3; // ignore 3 LSB
            float voltage = voltage_intval * 0.004; // LSB = 4 mV
            voltageData.push_back(voltage);

            timePrev = time;
            time = timeMs();
        }

        if ((secondsSinceEpoch() - timeLastMessage) > EVENT_TIMEOUT) {
            timeoutExpired = true;
            timeLastMessage = secondsSinceEpoch();
        }

        // send data
        if (currentData.size() > 2) {
            time_point input = std::chrono::system_clock::now();
            std::string dateString = serializeTimePoint(input, "%Y-%m-%d %H:%M:%S");
            float I_avg = std::reduce(currentData.begin(), currentData.end())
                / currentData.size();
            float V_avg = std::reduce(voltageData.begin(), voltageData.end())
                / voltageData.size();
            float I_std = calculateStddev(currentData);
            float V_min = *min_element(voltageData.begin(), voltageData.end());
            float V_max = *max_element(voltageData.begin(), voltageData.end());
            float I_min = *min_element(currentData.begin(), currentData.end());
            float I_max = *max_element(currentData.begin(), currentData.end());

            try {
                bool json_data_changed = false;

                if (abs(V_min - V_min_p) > EVENT_THRESHOLD || timeoutExpired) {
                    voltage_min_topic.publish(std::move(std::to_string(V_min)));
                    V_min_p = V_min;
                }
                if (abs(V_max - V_max_p) > EVENT_THRESHOLD || timeoutExpired) {
                    voltage_max_topic.publish(std::move(std::to_string(V_max)));
                    V_max_p = V_max;
                }
                if (abs(V_avg - V_avg_p) > EVENT_THRESHOLD || timeoutExpired) {
                    json_data_changed = true;
                    voltage_avg_topic.publish(std::move(std::to_string(V_avg)));
                    V_avg_p = V_avg;
                }
                if (abs(I_min - I_min_p) > EVENT_THRESHOLD || timeoutExpired) {
                    current_min_topic.publish(std::move(std::to_string(I_min)));
                    I_min_p = I_min;
                }
                if (abs(I_max - I_max_p) > EVENT_THRESHOLD || timeoutExpired) {
                    current_max_topic.publish(std::move(std::to_string(I_max)));
                    I_max_p = I_max;
                }
                if (abs(I_avg - I_avg_p) > EVENT_THRESHOLD || timeoutExpired) {
                    json_data_changed = true;
                    current_avg_topic.publish(std::move(std::to_string(I_avg)));
                    I_avg_p = I_avg;
                }
                if (abs(I_std - I_std_p) > EVENT_THRESHOLD || timeoutExpired) {
                    json_data_changed = true;
                    current_stddev_topic.publish(
                        std::move(std::to_string(I_std)));
                    I_std_p = I_std;
                }

                if (json_data_changed) {
                    json_message_id++;
                    std::string mqtt_payload = "{\"voltage\":"
                        + std::to_string(V_avg) + ", \"current\":"
                        + std::to_string(I_avg) + ", \"stddev\":"
                        + std::to_string(I_std) + ", \"messageCounter\":"
                        + std::to_string(json_message_id)
                        + ", \"timestamp\": \"" + dateString + "\""
                        + ", \"secondsSinceEpoch\":"
                        + std::to_string(secondsSinceEpoch()) + "}";

                    json_topic.publish(std::move(mqtt_payload));
                    timeLastMessage = secondsSinceEpoch();
                    timeoutExpired = false;
                }
            }
            catch (const mqtt::exception& exc) {
                std::cerr << exc.what() << std::endl;
            }
        }
    }
}

void pollSlaveAndWakeupIfNeccessary(monitor& mon)
{
    // TODO: refactor this
    auto slaveDebug = mon.getRadio<>(UartCommandDebug(), static_cast<std::chrono::milliseconds>(2000));

    if (slaveDebug.getReplyStatus() != UartCommandBase::ReplyStatus::Complete) {
        auto wakeup = mon.get<>(UartCommandWakeup(), static_cast<std::chrono::milliseconds>(12000));
    }

    auto slaveDeviceInfo = mon.getRadio<>(UartCommandGetDeviceInfo(), static_cast<std::chrono::milliseconds>(2000));

    if (slaveDeviceInfo.getReplyStatus() != UartCommandBase::ReplyStatus::Complete) {
        auto wakeup = mon.get<>(UartCommandWakeup(), static_cast<std::chrono::milliseconds>(12000));
    }
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

std::string getMasterNameAndPublishBirth(monitor& mon, mqtt::async_client& mqtt_client)
{
    std::string masterName("");

    auto masterDeviceInfo = mon.get<>(UartCommandGetDeviceInfo());
    if (masterDeviceInfo.getReplyStatus() == UartCommandBase::ReplyStatus::Complete) {
        auto response = masterDeviceInfo.responseStruct();
        time_point input = std::chrono::system_clock::now();
        std::string dateString = serializeTimePoint(input, "%Y-%m-%d %H:%M:%S");

        for (int i = 0; i < 16 && response.name[i] != 0; i++) {
            masterName += response.name[i];
        }

        mqtt::topic master_birth(
            mqtt_client, createMqttTopic("NBIRTH", masterName, ""), 0, false);
        master_birth.publish(std::move(dateString));
    }

    return (masterName);
}

std::string getSlaveNameAndPublishBirth(monitor& mon, mqtt::async_client& mqtt_client, std::string& masterName)
{
    std::string slaveName("");

    pollSlaveAndWakeupIfNeccessary(mon);

    auto slaveDeviceInfo = mon.getRadio<>(UartCommandGetDeviceInfo());
    if (slaveDeviceInfo.getReplyStatus() == UartCommandBase::ReplyStatus::Complete) {
        auto response = slaveDeviceInfo.responseStruct();
        time_point input = std::chrono::system_clock::now();
        std::string dateString = serializeTimePoint(input, "%Y-%m-%d %H:%M:%S");

        for (int i = 0; i < 16 && response.name[i] != 0; i++) {
            slaveName += response.name[i];
        }

        mqtt::topic slave_birth(
            mqtt_client, createMqttTopic("DBIRTH", masterName, slaveName), 0, false);
        slave_birth.publish(std::move(dateString));
    }
    // else {
    // auto wakeup = mon.get<>(UartCommandWakeup(), static_cast<std::chrono::milliseconds>(12000));
    // connection_attempts++;
    // mqtt::topic connection_attempts_topic(
    // mqtt_client, createMqttTopic("NDATA", masterName, "radio/wakeup_attempts"), 0, false);
    // connection_attempts_topic.publish(std::move(std::to_string(connection_attempts)));
    //}

    return (slaveName);
}

void readVccFromRadioSlaveAndPublish(monitor& mon, mqtt::async_client& mqtt_client, std::string& masterName, std::string& slaveName)
{
    pollSlaveAndWakeupIfNeccessary(mon);

    auto slaveVcc = mon.getRadio<>(UartCommandVcc());
    if (slaveVcc.getReplyStatus() == UartCommandBase::ReplyStatus::Complete) {
        int QOS = 0;
        mqtt::topic json_topic(mqtt_client, createMqttTopic("DDATA", masterName, slaveName), QOS, false);
        time_point input = std::chrono::system_clock::now();
        std::string dateString = serializeTimePoint(input, "%Y-%m-%d %H:%M:%S");

        uint16_t vcc_mv = (uint16_t)(slaveVcc.responseStruct().vcc_h << 8)
            | slaveVcc.responseStruct().vcc_l;

        try {
            std::string mqtt_payload
                = "{\"timestamp\": \"" + dateString + "\", \"voltage\":" + std::to_string(vcc_mv / 1000.0) + "}";

            json_topic.publish(std::move(mqtt_payload));
        }
        catch (const mqtt::exception& exc) {
            std::cerr << exc.what() << std::endl;
        }
    }
}

void parseMqttCommands(monitor& mon, mqtt::async_client& mqtt_client)
{
    std::string masterName;
    std::string slaveName;
    int connection_attempts = 0;
    const int QOS = 0;

    while (true) {
        // publishMonitorProtocolStatistics(mon, mqtt_client, masterName);

        if (masterName.empty()) {
            masterName = getMasterNameAndPublishBirth(mon, mqtt_client);
        }
        else if (slaveName.empty()) {
            slaveName = getSlaveNameAndPublishBirth(mon, mqtt_client, masterName);
        }
        else {
            std::string commandParamString;
            bool validCommand = false;

            try {
                std::string commandTopic = createMqttTopic("RCMD", masterName, "");
                int QOS = 0;
                mqtt_client.subscribe(commandTopic, QOS)->wait();

                std::cout << "\nWaiting for messages on topic: '" << commandTopic << "'" << std::endl;

                auto msg = mqtt_client.consume_message();

                if (msg) {
                    std::string json_string = msg->to_string();
                    // std::cout << "DEBUG: " << msg->get_topic() << ": " << json_string << std::endl;

                    auto jsonData = json::parse(json_string);
                    commandParamString = jsonData["command"];
                    validCommand = true;
                }
                else {
                    std::cout << "*** Connection Lost ***" << std::endl;
                }
            }
            catch (int e) {
            }

            // seher
            if (validCommand) {
                if (commandParamString == "getVcc") {
                    readVccFromRadioSlaveAndPublish(mon, mqtt_client, masterName, slaveName);
                }
            }
        }
    }
}

void readVccFromRadioSlave(monitor& mon, mqtt::async_client& mqtt_client)
{
    std::string masterName;
    std::string slaveName;
    int connection_attempts = 0;
    const int QOS = 0;

    while (true) {
        time_point input = std::chrono::system_clock::now();
        std::string dateString = serializeTimePoint(input, "%Y-%m-%d %H:%M:%S");

        // publishMonitorProtocolStatistics(mon, mqtt_client, masterName);

        if (masterName.empty()) {
            masterName = getMasterNameAndPublishBirth(mon, mqtt_client);
        }
        else if (slaveName.empty()) {
            slaveName = getSlaveNameAndPublishBirth(mon, mqtt_client, masterName);
        }
        else {

            if (!masterName.empty()) {
                auto slaveVcc = mon.getRadio<>(UartCommandVcc());

                if (slaveVcc.getReplyStatus() == UartCommandBase::ReplyStatus::Complete) {
                    mqtt::topic json_topic(mqtt_client, createMqttTopic("DDATA", masterName, slaveName), QOS, false);
                    // mqtt::topic vcc_topic(
                    // mqtt_client, createMqttTopic("DDATA", masterName, slaveName + "/vcc/voltage"), QOS, false);

                    uint16_t vcc_mv = (uint16_t)(slaveVcc.responseStruct().vcc_h << 8)
                        | slaveVcc.responseStruct().vcc_l;

                    try {
                        std::string mqtt_payload
                            = "{\"timestamp\": \"" + dateString + "\", \"voltage\":" + std::to_string(vcc_mv / 1000.0) + "}";

                        json_topic.publish(std::move(mqtt_payload));
                        // vcc_topic.publish(std::move(std::to_string(vcc_mv / 1000.0)));
                    }
                    catch (const mqtt::exception& exc) {
                        std::cerr << exc.what() << std::endl;
                    }

                    auto delayResponse = mon.getRadio<>(
                        UartCommandSleep(60000),
                        static_cast<std::chrono::milliseconds>(62000));
                }
                else {
                    mqtt::topic slave_death(
                        mqtt_client, createMqttTopic("DDEATH", masterName, slaveName), QOS, false);
                    slave_death.publish(std::move(dateString));

                    slaveName.clear();
                    auto wakeup = mon.get<>(UartCommandWakeup(), static_cast<std::chrono::milliseconds>(12000));
                    connection_attempts++;
                    mqtt::topic connection_attempts_topic(
                        mqtt_client, createMqttTopic("NDATA", masterName, "radio/wakeup_attempts"), QOS, false);
                    connection_attempts_topic.publish(std::move(std::to_string(connection_attempts)));
                }
            }
        }
    }
}

void parseOpt(int argc, char* argv[], monitor& mon)
{
    const std::string DFLT_ADDRESS { "tcp://localhost:1883" };
    // const std::string PERSIST_DIR { "data-persist" };
    const auto PERIOD = std::chrono::seconds(5);
    const int MAX_BUFFERED_MSGS = 120; // 120 * 5sec => 10min off-line buffering
                                       //
    std::string address = DFLT_ADDRESS;
    mqtt::async_client mqtt_client(address, "", MAX_BUFFERED_MSGS, 0);

    mqtt::connect_options connOpts;
    connOpts.set_keep_alive_interval(MAX_BUFFERED_MSGS * PERIOD);
    connOpts.set_clean_session(true);
    connOpts.set_automatic_reconnect(true);

    mqtt_client.start_consuming();

    mqtt_client.connect(connOpts)->wait();

    char option = 0;

    while ((option = getopt(argc, argv, "N:jhc")) != -1) {
        switch (option) {
        case 'N':
            readCurrentAndVoltage(mon, mqtt_client, atoi(optarg));
            break;
        case 'j':
            readVccFromRadioSlave(mon, mqtt_client);
            break;
        case 'c':
            parseMqttCommands(mon, mqtt_client);
            break;
        case 'h':
            print_usage();
            break;
        }
    }

    mqtt_client.disconnect()->wait();

    exit(0);
}

int main(int argc, char* argv[])
{
    std::string deviceFile { "/dev/ttyUSB0" };
    std::string deviceFileOption { "--device" };

    for (int i = 0; i < argc - 1; i++) {
        if (argv[i] == deviceFileOption) {
            std::string tmp = argv[i + 1];
            if (std::filesystem::exists(tmp)) {
                deviceFile = tmp;
                argv[i][0] = '\0';
                argv[i + 1][0] = '\0';
            }
        }
    }

    Uart uart(deviceFile);
    EventProcess ep;
    LinuxCryptoHandler cryptoHandler;
    monitor mon(uart, ep, &cryptoHandler);

    std::thread readerThread(&EventProcess::Run, &ep);

    parseOpt(argc, argv, mon);

    readerThread.join();

    return 0;
}
