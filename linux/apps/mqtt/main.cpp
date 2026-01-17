#include "mqtt/async_client.h"
#include <cmath>
#include <cmd/commands.hxx>
#include <desiredState.hpp>
#include <desiredStateCallback.hpp>
#include <deviceController.hpp>
#include <eventprocess.hpp>
#include <filesystem>
#include <linuxCryptoHandler.hpp>
#include <monitor.hpp>
#include <mqtt-common.hpp>
#include <radioSession.hpp>
#include <string>
#include <thread>
#include <uart.hpp>

using namespace std::chrono_literals;

void registerRadioNode(
    monitor& mon,
    mqtt::async_client& mqtt_client,
    uint32_t nodeAddress,
    std::vector<std::shared_ptr<DesiredState>>& desiredStateList,
    std::vector<std::shared_ptr<DeviceController>>& deviceControllerList,
    DesiredStateCallback& desiredStateCallback)
{
    bool isNewNode = true;

    for (auto it = deviceControllerList.begin(); it != deviceControllerList.end(); ++it) {
        if ((*it)->getNodeAddress() == nodeAddress) {
            isNewNode = false;
        }
    }

    if (isNewNode) {
        std::cout << "registerRadioNode:" << std::to_string(nodeAddress) << std::endl;
        RadioSession radioSession(mon, nodeAddress);
        radioSession.wakeupNotResponding();
        std::string nodeName = mon.getRadio<>(RaduinoCommandGetDeviceName()).getNamestring();

        if (!nodeName.empty()) {
            publishNbirth(mqtt_client, nodeName);
            DeviceController controller(mon, mqtt_client, nodeAddress, nodeName);

            desiredStateList.push_back(controller.getDesiredState());
            deviceControllerList.push_back(std::make_shared<DeviceController>(controller));
            desiredStateCallback.addDesiredState(controller.getDesiredState());
        }
    }
}

void readMultipleRadioNodes(monitor& mon, mqtt::async_client& mqtt_client)
{
    const int QOS = 0;

    getGatewayNameAndPublishBirth(mon, mqtt_client);

    std::vector<std::shared_ptr<DesiredState>> desiredStateList;
    std::vector<std::shared_ptr<DeviceController>> deviceControllerList;
    DesiredStateCallback desiredStateCallback(desiredStateList);

    mqtt_client.set_callback(desiredStateCallback);
    std::string commandTopic1 = "radio-arduino/RCMD/#";
    std::cout << "subscribe to topic: " << commandTopic1 << std::endl;
    mqtt_client.subscribe(commandTopic1, QOS)->wait();

    while (true) {
        for (std::shared_ptr<DeviceController> deviceController : deviceControllerList) {
            deviceController->execute();
            std::this_thread::sleep_for(1000ms);
        }
        // std::this_thread::sleep_for(100ms);
        uint32_t lastNode = mon.get<>(RaduinoCommandGetLastDeviceIdSeen()).responseStruct().getId();
        if (lastNode != 0) {
            registerRadioNode(mon, mqtt_client, lastNode, desiredStateList, deviceControllerList, desiredStateCallback);
        }
    }
}

void print_usage()
{
    std::cout << "raduino-mqtt-client" << std::endl;
    std::cout << "           -K <key> : encrypt command with transport key" << std::endl;
    std::cout << "                 -h : p rint this text" << std::endl;
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

    publishGatewayInfo(mqtt_client);

    char option = 0;

    while ((option = getopt(argc, argv, "K:h")) != -1) {
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
        case 'h':
            print_usage();
            break;
        }
    }

    readMultipleRadioNodes(mon, mqtt_client);

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
