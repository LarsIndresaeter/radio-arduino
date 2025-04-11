#include <desiredState.hpp>
#include <desiredStateCallback.hpp>
#include <digitalTwin.hpp>
#include "mqtt/async_client.h"
#include <mqtt_common.hpp>
#include <radioSession.hpp>
#include <chrono>
#include <cmath>
#include <commands.hpp>
#include <eventprocess.hpp>
#include <filesystem>
#include <iomanip>
#include <linuxCryptoHandler.hpp>
#include <monitor.hpp>
#include <numeric>
#include <sstream>
#include <string>
#include <thread>
#include <uart.hpp>

using namespace std::chrono_literals;

void registerRadioNode(monitor& mon, mqtt::async_client& mqtt_client, uint8_t nodeAddress, std::vector<std::shared_ptr<DesiredState>>& desiredStateList, std::vector<std::shared_ptr<DigitalTwin>>& digitalTwinList)
{
    RadioSession radioSession(mon, nodeAddress);
    radioSession.wakeupNotResponding();
    std::string nodeName = radioSession.getNodeNameAndPublishBirth(mqtt_client);

    if (!nodeName.empty()) {
        DigitalTwin twin(mon, mqtt_client, nodeAddress, nodeName);

        desiredStateList.push_back(twin.getDesiredState());
        digitalTwinList.push_back(std::make_shared<DigitalTwin>(twin));
    }
}

void moveRadioNode(monitor& mon, mqtt::async_client& mqtt_client, std::string name, uint8_t nodeAddress)
{
    RadioSession radioSession(mon, 0);
    radioSession.wakeupNotResponding();
    std::string nodeName = radioSession.getNodeNameAndPublishBirth(mqtt_client);

    if (nodeName == name) {
        mon.getRadio<>(UartCommandSetNodeAddress(nodeAddress));
    }
}

void readMultipleRadioNodes(monitor& mon, mqtt::async_client& mqtt_client, std::vector<uint8_t> nodeAddressList)
{
    const int QOS = 0;

    std::vector<std::shared_ptr<DesiredState>> desiredStateList;
    std::vector<std::shared_ptr<DigitalTwin>> digitalTwinList;

    getGatewayNameAndPublishBirth(mon, mqtt_client);

    //moveRadioNode(mon, "solar-lamp", 100);
    //moveRadioNode(mon, "breadboard", 101);
    //moveRadioNode(mon, mqtt_client, "lcd", 102);

    //nodeAddressList.push_back(100);
    //nodeAddressList.push_back(101);
    //nodeAddressList.push_back(102);

    for (uint8_t i = 0; i < nodeAddressList.size(); i++) {
        registerRadioNode(mon, mqtt_client, nodeAddressList.at(i), desiredStateList, digitalTwinList);
    }

    DesiredStateCallback desiredStateCallback(desiredStateList);

    mqtt_client.set_callback(desiredStateCallback);
    std::string commandTopic1 = "radio-arduino/RCMD/#";
    std::cout << "subscribe to topic: " << commandTopic1 << std::endl;
    mqtt_client.subscribe(commandTopic1, QOS)->wait();

    while (true) {
        for (int i = 0; i < desiredStateList.size(); i++) {
            digitalTwinList.at(i)->execute();
        }
        std::this_thread::sleep_for(100ms);
    }
}

void print_usage()
{
    std::cout << "mqtt-client" << std::endl;
    std::cout << "       -k : read voltage from rf node" << std::endl;
    std::cout << "       -n : gateway address" << std::endl;
    std::cout << "       -h : print this text" << std::endl;
    std::cout << std::endl;
}

void parseOpt(int argc, char* argv[], monitor& mon)
{
    const std::string DFLT_ADDRESS { "tcp://localhost:1883" };
    const auto PERIOD = std::chrono::seconds(5);
    const int MAX_BUFFERED_MSGS = 120;

    std::string address = DFLT_ADDRESS;
    mqtt::async_client mqtt_client(address, "raduino-client", MAX_BUFFERED_MSGS, 0);

    mqtt::connect_options connOpts;
    connOpts.set_keep_alive_interval(MAX_BUFFERED_MSGS * PERIOD);
    connOpts.set_clean_session(true);
    connOpts.set_automatic_reconnect(true);

    mqtt_client.start_consuming();

    mqtt_client.connect(connOpts)->wait();

    std::vector<uint8_t> nodeAddressList;
    char option = 0;

    while ((option = getopt(argc, argv, "hn:")) != -1) {
        switch (option) {
        case 'n':
            nodeAddressList.push_back(atoi(optarg));
            break;
        case 'h':
            print_usage();
            break;
        }
    }

    readMultipleRadioNodes(mon, mqtt_client, nodeAddressList);

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
