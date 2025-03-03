
#include "mqtt/async_client.h"
#include <chrono>
#include <cmath>
#include <commands.hpp>
#include <eventprocess.hpp>
#include <filesystem>
#include <linuxCryptoHandler.hpp>
#include <monitor.hpp>
#include <numeric>
#include <thread>
#include <uart.hpp>
#include <string>

using namespace std::chrono_literals;

constexpr float EVENT_THRESHOLD = 0.01; // 1% change before sending new message
constexpr uint EVENT_TIMEOUT = 10 * 60; // seconds before sending new message

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
    std::cout << "mqtt-tool" << std::endl;
    std::cout << "       -M : ina219 power monitor" << std::endl;
    std::cout << "       -N : ina219 power monitor, statistic per second for "
                 "<N> seconds"
              << std::endl;
    std::cout << "       -j : read voltage from rf slave" << std::endl;
    std::cout << "       -h : print this text" << std::endl;
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

void readCurrentAndVoltage(monitor& mon, int samples)
{
    auto ina219 = mon.get<>(UartCommandIna219());
    std::vector<float> currentData, voltageData;

    const std::string DFLT_ADDRESS { "tcp://localhost:1883" };
    const int QOS = 0;
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

    mqtt::topic json_topic(mqtt_client, "sensor/power/ina219/all", QOS, false);
    mqtt::topic current_min_topic(
        mqtt_client, "sensor/power/ina219/current/min", QOS, false);
    mqtt::topic current_max_topic(
        mqtt_client, "sensor/power/ina219/current/max", QOS, false);
    mqtt::topic current_avg_topic(
        mqtt_client, "sensor/power/ina219/current/avg", QOS, false);
    mqtt::topic current_stddev_topic(
        mqtt_client, "sensor/power/ina219/current/stddev", QOS, false);
    mqtt::topic voltage_min_topic(
        mqtt_client, "sensor/power/ina219/voltage/min", QOS, false);
    mqtt::topic voltage_max_topic(
        mqtt_client, "sensor/power/ina219/voltage/max", QOS, false);
    mqtt::topic voltage_avg_topic(
        mqtt_client, "sensor/power/ina219/voltage/avg", QOS, false);
    mqtt::topic device_name_topic(
        mqtt_client, "sensor/power/ina219/name", QOS, false);

    mqtt_client.connect(connOpts)->wait();

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

    
    for (int attempts = 0; attempts <= 10; attempts++) 
    {
        auto deviceInfo = mon.get<>(UartCommandGetDeviceInfo());

        if (deviceInfo.getReplyStatus() == UartCommandBase::ReplyStatus::Complete) {
            try {
                auto response = deviceInfo.responseStruct();
                std::string name;

                for (int i = 0; i < 16 && response.name[i] != 0; i++) {
                    name += response.name[i];
                }

                device_name_topic.publish(name);
                break; // successfully read name and published it to the mqtt broker
            }
            catch (const mqtt::exception& exc) {
                std::cerr << exc.what() << std::endl;
            }
        }
    }

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
            voltage_intval = voltage_intval >> 3;   // ignore 3 LSB
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

    mqtt_client.disconnect()->wait();
}

void readVccFromRadioSlave(monitor& mon)
{
    const std::string DFLT_ADDRESS { "tcp://localhost:1883" };
    const int QOS = 0;
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

    mqtt::topic json_topic(mqtt_client, "arduino/sensor/slave/vcc/json", QOS, false);
    mqtt::topic vcc_topic(
        mqtt_client, "arduino/sensor/slave/vcc/voltage", QOS, false);
    mqtt::topic connection_status_topic(
        mqtt_client, "arduino/radio/status", QOS, false);
    mqtt::topic connection_attempts_topic(
        mqtt_client, "arduino/radio/wakeup_attempts", QOS, false);
    int connection_attempts = 0;

    mqtt::topic link_commands_sent_topic(
        mqtt_client, "arduino/uart/commands_sent", QOS, false);
    mqtt::topic link_valid_responses_topic(
        mqtt_client, "arduino/uart/valid_responses", QOS, false);
    mqtt::topic link_invalid_responses_topic(
        mqtt_client, "arduino/uart/invalid_responses", QOS, false);
    mqtt::topic link_bytes_sent_topic(
        mqtt_client, "arduino/uart/bytes_sent", QOS, false);
    mqtt::topic link_bytes_received_topic(
        mqtt_client, "arduino/uart/bytes_received", QOS, false);
    mqtt::topic slave_name(
        mqtt_client, "arduino/slave/name", QOS, false);
    mqtt::topic master_name(
        mqtt_client, "arduino/master/name", QOS, false);
    
    mqtt_client.connect(connOpts)->wait();

    connection_attempts_topic.publish(std::move(std::to_string(connection_attempts)));

    while (true) {

        link_commands_sent_topic.publish(std::move(std::to_string(mon.getCommandsSent())));
        link_valid_responses_topic.publish(std::move(std::to_string(mon.getValidResponses())));
        link_invalid_responses_topic.publish(std::move(std::to_string(mon.getInvalidResponses())));
        link_bytes_sent_topic.publish(std::move(std::to_string(mon.getBytesSent())));
        link_bytes_received_topic.publish(std::move(std::to_string(mon.getBytesReceived())));


        auto slaveDeviceInfo =  mon.getRadio<>(UartCommandGetDeviceInfo());
        if (slaveDeviceInfo.getReplyStatus() == UartCommandBase::ReplyStatus::Complete) {
                auto response = slaveDeviceInfo.responseStruct();
                std::string name;

                for (int i = 0; i < 16 && response.name[i] != 0; i++) {
                    name += response.name[i];
                }

            slave_name.publish(std::move(name));
        }

        auto masterDeviceInfo =  mon.get<>(UartCommandGetDeviceInfo());
        if (masterDeviceInfo.getReplyStatus() == UartCommandBase::ReplyStatus::Complete) {
                auto response = masterDeviceInfo.responseStruct();
                std::string name;

                for (int i = 0; i < 16 && response.name[i] != 0; i++) {
                    name += response.name[i];
                }

            master_name.publish(std::move(name));
        }

        auto vcc = mon.getRadio<>(UartCommandVcc());

        if (vcc.getReplyStatus() == UartCommandBase::ReplyStatus::Complete) {
            connection_status_topic.publish("connected");

            uint16_t vcc_mv = (uint16_t)(vcc.responseStruct().vcc_h << 8)
                | vcc.responseStruct().vcc_l;

            try {
                std::string mqtt_payload
                    = "{\"voltage\":" + std::to_string(vcc_mv / 1000.0) + "}";

                json_topic.publish(std::move(mqtt_payload));
                vcc_topic.publish(std::move(std::to_string(vcc_mv / 1000.0)));
            }
            catch (const mqtt::exception& exc) {
                std::cerr << exc.what() << std::endl;
            }

            // std::this_thread::sleep_for(500 * 1ms);

            auto delayResponse = mon.getRadio<>(
                UartCommandSleep(60000),
                static_cast<std::chrono::milliseconds>(62000));
        }
        else
        {
            connection_status_topic.publish("disconnected");
            auto wakeup = mon.get<>(UartCommandWakeup(), static_cast<std::chrono::milliseconds>(12000));
            connection_attempts++;
            connection_attempts_topic.publish(std::move(std::to_string(connection_attempts)));
        }
    }

    mqtt_client.disconnect()->wait();
}

void parseOpt(int argc, char* argv[], monitor& mon)
{
    char option = 0;

    while ((option = getopt(argc, argv, "N:jh")) != -1) {
        switch (option) {
        case 'N':
            readCurrentAndVoltage(mon, atoi(optarg));
            break;
        case 'j': {
            readVccFromRadioSlave(mon);
        } break;
        case 'h':
            print_usage();
            break;
        }
    }

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
