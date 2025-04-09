#include "desiredStateConfiguration.hpp"
#include "digitalTwin.hpp"
#include "mqtt/async_client.h"
#include "mqtt_common.hpp"
#include "radioSession.hpp"
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

void registerRadioSlave(monitor& mon, mqtt::async_client& mqtt_client, uint8_t slaveAddress, std::vector<std::shared_ptr<DesiredStateConfiguration>>& desiredStateList, std::vector<std::shared_ptr<DigitalTwin>>& digitalTwinList)
{
    RadioSession radioSession(mon, slaveAddress);
    radioSession.wakeupNotResponding();
    std::string slaveName = radioSession.getSlaveNameAndPublishBirth(mqtt_client);

    if (!slaveName.empty()) {
        DigitalTwin twin(mon, mqtt_client, slaveAddress, slaveName);

        desiredStateList.push_back(twin.getDesiredStateConfiguration());
        digitalTwinList.push_back(std::make_shared<DigitalTwin>(twin));
    }
}

void moveRadioSlave(monitor& mon, mqtt::async_client& mqtt_client, std::string name, uint8_t slaveAddress)
{
    RadioSession radioSession(mon, 0);
    radioSession.wakeupNotResponding();
    std::string slaveName = radioSession.getSlaveNameAndPublishBirth(mqtt_client);

    if (slaveName == name) {
        mon.getRadio<>(UartCommandSetSlaveAddress(slaveAddress));
    }
}

void readMultipleRadioSlaves(monitor& mon, mqtt::async_client& mqtt_client, std::vector<uint8_t> slaveList)
{
    const int QOS = 0;

    std::vector<std::shared_ptr<DesiredStateConfiguration>> desiredStateList;
    std::vector<std::shared_ptr<DigitalTwin>> digitalTwinList;

    std::string masterName = getMasterNameAndPublishBirth(mon, mqtt_client);

    //moveRadioSlave(mon, "solar-lamp", 100);
    //moveRadioSlave(mon, "breadboard", 101);
    //moveRadioSlave(mon, mqtt_client, "lcd", 102);

    //slaveList.push_back(100);
    //slaveList.push_back(101);
    //slaveList.push_back(102);

    for (uint8_t i = 0; i < slaveList.size(); i++) {
        registerRadioSlave(mon, mqtt_client, slaveList.at(i), desiredStateList, digitalTwinList);
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
    std::cout << "       -k : read voltage from rf slave" << std::endl;
    std::cout << "       -m : master address" << std::endl;
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

    std::vector<uint8_t> slaveList;
    char option = 0;

    while ((option = getopt(argc, argv, "hm:")) != -1) {
        switch (option) {
        case 'm':
            slaveList.push_back(atoi(optarg));
            break;
        case 'h':
            print_usage();
            break;
        }
    }

    readMultipleRadioSlaves(mon, mqtt_client, slaveList);

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
