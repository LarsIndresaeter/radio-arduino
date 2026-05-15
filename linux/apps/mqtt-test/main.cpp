#include "mqtt/async_client.h"
#include <nlohmann/json.hpp>
#include <string>
#include <thread>

using nlohmann::json;

using namespace std::chrono_literals;
using namespace std::chrono;

void print_usage()
{
    std::cout << "raduino-mqtt-test" << std::endl;
    std::cout << "        -a <address>: nodeAddress (used by resendBirthCertificate and subscription)" << std::endl;
    std::cout << "                 -b : resendBirthCertificate" << std::endl;
    std::cout << "          -s <type> : subscription type (vcc, gpio, quadrature_encoder, lsm303d)" << std::endl;
    std::cout << "                 -l : command list" << std::endl;
    std::cout << "          -t <text> : print text on ssd1306 lcd display" << std::endl;
    std::cout << "                 -h : print this text" << std::endl;
}

uint64_t getTimestamp()
{
    uint64_t timestamp = (duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count());
    return (timestamp);
}

void publishMessage(mqtt::async_client& mqtt_client, std::string topic, std::string message)
{
    const int QOS = 0;
    mqtt::topic mqttTopic(mqtt_client, topic, QOS, false);

    try {
        mqttTopic.publish(std::move(message));
    }
    catch (std::exception const& e) {
        std::cerr << e.what() << std::endl;
        std::cerr << "topic: " << topic << std::endl;
        std::cerr << "message: " << message << std::endl;
    }
}

void resendBirthCertificate(mqtt::async_client& mqtt_client)
{
    std::string topic = "raduino-adapter/RCMD";

    json command;
    command["command"] = "resendBirthCertificate";
    command["params"]["timestamp"] = getTimestamp();

    publishMessage(mqtt_client, topic, command.dump());
}

void batchCommandStaticNodeInfo(mqtt::async_client& mqtt_client, uint64_t nodeAddress)
{
    std::string topic = "raduino-router/RCMD/" + std::to_string(nodeAddress);

    json commandList;
    commandList.push_back("vcc");
    commandList.push_back("get_version");
    commandList.push_back("get_device_name");
    commandList.push_back("get_statistics");
    commandList.push_back("get_lsm303d");
    commandList.push_back("get_attached_devices_csv_string");
    commandList.push_back("get_active_time_counter");
    commandList.push_back("quadrature_encoder");
    commandList.push_back("gpio");

    json command;
    command["command"] = "batchCommand";
    command["params"]["commandList"] = commandList;
    command["params"]["timestamp"] = getTimestamp();
    command["params"]["nodeAddress"] = nodeAddress;

    publishMessage(mqtt_client, topic, command.dump());
}

void setTextOnLcd(mqtt::async_client& mqtt_client, uint64_t nodeAddress, std::string displayText)
{
    std::string topic = "raduino-router/RCMD/" + std::to_string(nodeAddress);

    json commandList;
    commandList.push_back("ssd1306");

    json command;
    command["command"] = "batchCommand";
    command["params"]["displayText"] = displayText;
    command["params"]["commandList"] = commandList;
    command["params"]["timestamp"] = getTimestamp();
    command["params"]["nodeAddress"] = nodeAddress;

    publishMessage(mqtt_client, topic, command.dump());
}

void subscriptionCommand(mqtt::async_client& mqtt_client, uint64_t nodeAddress, uint8_t subscriptionId)
{
    std::string topic = "raduino-router/RCMD/" + std::to_string(nodeAddress);

    json command;
    command["command"] = "subscription";
    command["params"]["subscriptionId"] = subscriptionId;
    command["params"]["subscriptionInterval"] = 1;
    command["params"]["timestamp"] = getTimestamp();
    command["params"]["nodeAddress"] = nodeAddress;

    publishMessage(mqtt_client, topic, command.dump());
}

void parseOpt(int argc, char* argv[])
{
    const std::string DFLT_ADDRESS { "tcp://localhost:1883" };
    const auto PERIOD = std::chrono::seconds(5);
    const int MAX_BUFFERED_MSGS = 120;

    std::string address = DFLT_ADDRESS;
    mqtt::async_client mqtt_client(address, "", MAX_BUFFERED_MSGS, 0);

    mqtt::connect_options connOpts;
    connOpts.set_keep_alive_interval(MAX_BUFFERED_MSGS * PERIOD);
    connOpts.set_clean_session(true);
    connOpts.set_automatic_reconnect(true);

    mqtt_client.connect(connOpts)->wait();

    char option = 0;
    uint32_t nodeAddress = 0;

    while ((option = getopt(argc, argv, "bla:s:t:h")) != -1) {
        switch (option) {
        case 'b':
            resendBirthCertificate(mqtt_client);
            break;
        case 't':
            setTextOnLcd(mqtt_client, nodeAddress, optarg);
            break;
        case 'a':
            nodeAddress = atoi(optarg);
            break;
        case 's': {
            uint8_t id = 0;
            std::string type = optarg;
            if (type == "gpio") {
                id = 11;
            }
            else if (type == "vcc") {
                id = 28;
            }
            else if (type == "lsm303d") {
                id = 43;
            }
            else if (type == "quadrature_encoder") {
                id = 34;
            }

            if (id == 0) {
                std::cout << "type '" << type << "' is not a recognized command type" << std::endl;
                std::cout << "valid arguments are: vcc, gpio, quadrature_encoder and lsm303d" << std::endl;
            }
            else {
                subscriptionCommand(mqtt_client, nodeAddress, id);
            }
        } break;
        case 'l':
            batchCommandStaticNodeInfo(mqtt_client, nodeAddress);
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
    parseOpt(argc, argv);

    return 0;
}
