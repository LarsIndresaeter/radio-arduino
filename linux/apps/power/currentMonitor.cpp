#include "currentMonitor.hpp"

using nlohmann::json;

int timeMs()
{
    using namespace std::chrono;
    int ms
        = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())
                  .count())
        % 1000;
    return ms;
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

    std::string masterName = getMasterNameAndPublishBirth(mon, mqtt_client);

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

                    //json message = {
                        //{ "voltage", std::to_string(V_avg),
                            //"current", std::to_string(I_avg),
                            //"stddev", std::to_string(I_std),
                            //"messageCounter", std::to_string(json_message_id),
                            //"timestamp", getDateTimeString(),
                            //"secondsSinceEpoch", std::to_string(secondsSinceEpoch()) }
                    //};

                    std::string mqtt_payload = "{\"voltage\":"
                        + std::to_string(V_avg) + ", \"current\":"
                        + std::to_string(I_avg) + ", \"stddev\":"
                        + std::to_string(I_std) + ", \"messageCounter\":"
                        + std::to_string(json_message_id)
                        + ", \"timestamp\": \"" + getDateTimeString() + "\""
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


