#include "include/deviceController.hpp"
#include "mqtt/async_client.h"
#include <cmath>
#include <cmd/commands.hxx>
#include <commandCallback.hpp>
#include <deviceController.hpp>
#include <eventprocess.hpp>
#include <filesystem>
#include <linuxCryptoHandler.hpp>
#include <monitor.hpp>
#include <radioSession.hpp>
#include <string>
#include <thread>
#include <uart.hpp>

using namespace std::chrono_literals;
using namespace std::chrono;

void registerNode(
    monitor& mon,
    mqtt::async_client& mqtt_client,
    uint32_t nodeAddress,
    std::vector<std::shared_ptr<DeviceController>>& deviceControllerList,
    CommandCallback& commandCallback,
    uint32_t gatewayAddress)
{
    bool isNewNode = true;

    for (auto it = deviceControllerList.begin(); it != deviceControllerList.end(); ++it) {
        if ((*it)->getNodeAddress() == nodeAddress) {
            isNewNode = false;
        }
    }

    if (isNewNode) {
        std::cout << "register raduino node:" << std::to_string(nodeAddress) << std::endl;

        DeviceController controller(mon, mqtt_client, nodeAddress, gatewayAddress);
        std::shared_ptr<DeviceController> ctr = std::make_shared<DeviceController>(controller);

        deviceControllerList.push_back(ctr);
        commandCallback.addDeviceController(ctr);
    }
    else {
        for (std::shared_ptr<DeviceController> deviceController : deviceControllerList) {
            if (deviceController->getNodeAddress() == nodeAddress) {
                deviceController->discoveryReceived(nodeAddress);
            }
        }
    }
}

void readMultipleRadioNodes(monitor& mon, mqtt::async_client& mqtt_client)
{
    const int QOS = 0;

    uint64_t gatewayAddress = 0;

    for (int i = 0; i < 10; i++) {
        gatewayAddress = mon.get<>(RaduinoCommandGetUniqueId()).responseStruct().getId();
        if (gatewayAddress > 0) {
            break;
        }
        if(i>4)
        {
        std::cout << "DEBUG: read gateway address " << std::to_string(i) << " times" << std::endl;
        }
        std::this_thread::sleep_for(i * 100ms);
    }

    if (gatewayAddress == 0) {
        std::cout << "ERROR: not able to read gateway address" << std::endl;
        exit(1);
    }

    std::vector<std::shared_ptr<DeviceController>> deviceControllerList;
    CommandCallback commandCallback;

    mqtt_client.set_callback(commandCallback);
    std::string commandTopic1 = "raduino-bridge/RCMD/#";
    std::cout << "connected to gateway: " << std::to_string(gatewayAddress) << std::endl;
    mqtt_client.subscribe(commandTopic1, QOS)->wait();

    registerNode(mon, mqtt_client, gatewayAddress, deviceControllerList, commandCallback, gatewayAddress);

    uint64_t timestampLastGatewayAdvertisement = 0;

    while (true) {
        for (std::shared_ptr<DeviceController> deviceController : deviceControllerList) {
            deviceController->execute();

            uint32_t lastNode = deviceController->getLastDeviceIdSeen();

            if (lastNode != 0) {
                registerNode(mon, mqtt_client, lastNode, deviceControllerList, commandCallback, gatewayAddress);
            }

            std::this_thread::sleep_for(500ms);
        }

        //TODO: extract this to a function
        uint64_t timestampMsSinceEpoch = (duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count());
        if ((timestampMsSinceEpoch - timestampLastGatewayAdvertisement) > 5000) {
            timestampLastGatewayAdvertisement = timestampMsSinceEpoch;
            //std::cout << "DEBUG: publish discovery for gateway:" << std::to_string(gatewayAddress) << ", timestamp:" << std::to_string(timestampLastGatewayAdvertisement) << std::endl;

            for (std::shared_ptr<DeviceController> deviceController : deviceControllerList) {
                if (deviceController->getNodeAddress() == gatewayAddress) {
                    deviceController->discoveryReceived(gatewayAddress);
                }
            }
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
    mqtt::async_client mqtt_client(address, "", MAX_BUFFERED_MSGS, 0);

    mqtt::connect_options connOpts;
    connOpts.set_keep_alive_interval(MAX_BUFFERED_MSGS * PERIOD);
    connOpts.set_clean_session(true);
    connOpts.set_automatic_reconnect(true);

    mqtt_client.start_consuming();

    mqtt_client.connect(connOpts)->wait();

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
