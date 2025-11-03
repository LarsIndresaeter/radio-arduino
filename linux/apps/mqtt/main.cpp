#include "mqtt/async_client.h"
#include <chrono>
#include <cmath>
#include <cmd/commands.hxx>
#include <desiredState.hpp>
#include <desiredStateCallback.hpp>
#include <digitalTwin.hpp>
#include <eventprocess.hpp>
#include <filesystem>
#include <iomanip>
#include <linuxCryptoHandler.hpp>
#include <monitor.hpp>
#include <mqtt-common.hpp>
#include <numeric>
#include <radioSession.hpp>
#include <sstream>
#include <string>
#include <thread>
#include <uart.hpp>

using namespace std::chrono_literals;

void registerRadioNode(
    monitor& mon,
    mqtt::async_client& mqtt_client,
    uint8_t nodeAddress,
    std::vector<std::shared_ptr<DesiredState>>& desiredStateList,
    std::vector<std::shared_ptr<DigitalTwin>>& digitalTwinList)
{
    RadioSession radioSession(mon, nodeAddress);
    radioSession.wakeupNotResponding();
    std::string nodeName = mon.getRadio<>(RaduinoCommandGetDeviceName()).getNamestring();

    if (!nodeName.empty()) {
        publishNbirth(mqtt_client, nodeName);
        DigitalTwin twin(mon, mqtt_client, nodeAddress, nodeName);

        desiredStateList.push_back(twin.getDesiredState());
        digitalTwinList.push_back(std::make_shared<DigitalTwin>(twin));
    }
}

void moveRadioNode(
    monitor& mon,
    mqtt::async_client& mqtt_client,
    std::vector<std::shared_ptr<DesiredState>>& desiredStateList,
    std::vector<std::shared_ptr<DigitalTwin>>& digitalTwinList,
    std::string name,
    uint8_t nodeAddress)
{
    RadioSession radioSession(mon, 0);
    radioSession.wakeupNotResponding();

    std::string nodeName = mon.getRadio<>(RaduinoCommandGetDeviceName()).getNamestring();

    if (nodeName == name) {
        std::cout << "DEBUG: try to move '" + nodeName + "' to address: " + std::to_string(nodeAddress) << std::endl;
        mon.getRadio<>(RaduinoCommandSetNodeAddress(nodeAddress));

        if (mon.lastCommandReturnedValidResponse()) {
            std::cout << "DEBUG: move operation was successful" << std::endl;
        }

        registerRadioNode(mon, mqtt_client, nodeAddress, desiredStateList, digitalTwinList);
    }
}

void readMultipleRadioNodes(monitor& mon, mqtt::async_client& mqtt_client, std::vector<uint8_t> nodeAddressList)
{
    const int QOS = 0;

    std::vector<std::shared_ptr<DesiredState>> desiredStateList;
    std::vector<std::shared_ptr<DigitalTwin>> digitalTwinList;

    getGatewayNameAndPublishBirth(mon, mqtt_client);

    // moveRadioNode(mon, mqtt_client, desiredStateList, digitalTwinList, "solar-lamp", 100);
    // moveRadioNode(mon, mqtt_client, desiredStateList, digitalTwinList, "breadboard", 101);
    // moveRadioNode(mon, mqtt_client, desiredStateList, digitalTwinList, "lcd", 102);

    for (uint8_t nodeAddress : nodeAddressList) {
        registerRadioNode(mon, mqtt_client, nodeAddress, desiredStateList, digitalTwinList);
    }

    DesiredStateCallback desiredStateCallback(desiredStateList);

    mqtt_client.set_callback(desiredStateCallback);
    std::string commandTopic1 = "radio-arduino/RCMD/#";
    std::cout << "subscribe to topic: " << commandTopic1 << std::endl;
    mqtt_client.subscribe(commandTopic1, QOS)->wait();

    while (true) {
        for (std::shared_ptr<DigitalTwin> digitalTwin : digitalTwinList) {
            digitalTwin->execute();
        }
        std::this_thread::sleep_for(100ms);
    }
}

void print_usage()
{
    std::cout << "raduino-mqtt-client" << std::endl;
    std::cout << "       -K : encrypt command with transport key" << std::endl;
    std::cout << "       -N : wakeup node address" << std::endl;
    std::cout << "       -n : gateway address" << std::endl;
    std::cout << "       -h : print this text" << std::endl;
    std::cout << std::endl;
}

void parseOpt(int argc, char* argv[], monitor& mon, LinuxCryptoHandler& cryptoHandler)
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
    uint8_t radioAddress = 0;

    while ((option = getopt(argc, argv, "K:N:hn:")) != -1) {
        switch (option) {
        case 'K': {
            std::string s(optarg);
            std::vector<uint8_t> key(16, 0);

            // read key ascii values
            for (uint8_t i = 0; i < s.size() && i < 16; i++) {
                key.at(i) = s.at(i);
            }

            // set key
            cryptoHandler.setTransportKey((uint8_t*)&key[0]);
            cryptoHandler.setMacKey((uint8_t*)&key[0]);
            mon.setTransportEncryption(true);
        } break;
        case 'N':
            radioAddress = atoi(optarg);
            {
                RadioSession radioSession(mon, radioAddress);
                radioSession.wakeupNotResponding();
            }
            break;

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

    parseOpt(argc, argv, mon, cryptoHandler);

    readerThread.join();

    return 0;
}
