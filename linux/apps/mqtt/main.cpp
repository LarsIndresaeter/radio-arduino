#include "currentMonitor.hpp"
#include "desiredStateConfiguration.hpp"
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

void pollRadioSlaveAndSetDesiredState(monitor& mon, mqtt::async_client& mqtt_client, std::shared_ptr<DesiredStateConfiguration> dsc)
{
    RadioSession radioSession(mon, dsc->getRadioAddress());

    if (dsc->getDesiredPollInterval() != dsc->getActualPollInterval()) {
        dsc->setActualPollInterval(dsc->getDesiredPollInterval());
        publishDesiredStatePollInterval(mqtt_client, dsc);
    }

    if ((secondsSinceEpoch() - dsc->getTimeLastPoll()) > dsc->getActualPollInterval()) {
        if(radioSession.wakeupNotResponding())
        {
            dsc->setTimeLastPoll(secondsSinceEpoch());
            std::string slaveName = dsc->getName();
            readVccAndPublish(mon, mqtt_client, slaveName); // TODO: return true if success
        }
    }

    if (dsc->displayTextChanged()) {
        if(radioSession.wakeupNotResponding())
        {
            radioSession.setKeepAliveInterval(50);
            updateDisplayText(mon, mqtt_client, dsc);
        }
    }
}

void readVccFromMultipleRadioSlave(monitor& mon, mqtt::async_client& mqtt_client, std::vector<uint8_t> slaveList)
{
    std::string masterName;
    const int QOS = 0;

    std::vector<std::shared_ptr<DesiredStateConfiguration>> desiredStateList;

    //desiredStateList.push_back(std::make_shared<DesiredStateConfiguration>(0, "lcd"));
    //desiredStateList.push_back(std::make_shared<DesiredStateConfiguration>(100, "solar-lamp"));
    //desiredStateList.push_back(std::make_shared<DesiredStateConfiguration>(101, "breadboard"));

    masterName = getMasterNameAndPublishBirth(mon, mqtt_client);

    for(uint8_t i = 0; i<slaveList.size(); i++)
    {
        uint8_t slaveAddress = slaveList.at(i);
        RadioSession radioSession(mon, slaveAddress);
        radioSession.wakeupNotResponding();
        std::string slaveName = radioSession.getSlaveNameAndPublishBirth(mqtt_client);
        //std::cout << "DEBUG: check for radio slave on address: " << std::to_string(slaveAddress) << std::endl;

        if(!slaveName.empty())
        {
            desiredStateList.push_back(std::make_shared<DesiredStateConfiguration>(slaveAddress, slaveName));
        }
    }

    DesiredState desiredStateCallback(desiredStateList);


    mqtt_client.set_callback(desiredStateCallback);
    std::string commandTopic1 = "radio-arduino/RCMD/#";
    std::cout << "subscribe to topic: " << commandTopic1 << std::endl;
    mqtt_client.subscribe(commandTopic1, QOS)->wait();

    while (true) {
        if (masterName.empty()) {
            masterName = getMasterNameAndPublishBirth(mon, mqtt_client);
        }
        else {
            for (int i = 0; i < desiredStateList.size(); i++) {
                std::shared_ptr<DesiredStateConfiguration> dsc = desiredStateList.at(i);

                pollRadioSlaveAndSetDesiredState(mon, mqtt_client, dsc);
            }
            std::this_thread::sleep_for(100ms);
        }
    }
}

void print_usage()
{
    std::cout << "mqtt-client" << std::endl;
    std::cout << "       -N : ina219 power monitor, statistic per second for "
                 "<N> seconds"
              << std::endl;
    std::cout << "       -j : read voltage from rf slave" << std::endl;
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

    while ((option = getopt(argc, argv, "N:khm:")) != -1) {
        switch (option) {
        case 'm':
            slaveList.push_back(atoi(optarg));
            break;
        case 'N':
            readCurrentAndVoltage(mon, mqtt_client, atoi(optarg));
            break;
        case 'k':
            readVccFromMultipleRadioSlave(mon, mqtt_client, slaveList);
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
