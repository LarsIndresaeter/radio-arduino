#include "currentMonitor.hpp"
#include "mqtt-common.hpp"
#include "mqtt/async_client.h"
#include <cmath>
#include <cmd/commands.hxx>
#include <eventprocess.hpp>
#include <filesystem>
#include <linuxCryptoHandler.hpp>
#include <monitor.hpp>
#include <radioSession.hpp>
#include <string>
#include <thread>
#include <uart.hpp>

using namespace std::chrono_literals;

void print_usage()
{
    std::cout << "power-monitor" << std::endl;
    std::cout << "           -K <key> : encrypt command with transport key" << std::endl;
    std::cout << "       -N <address> : wakeup node address" << std::endl;
    std::cout << "       -n <seconds> : ina219 power monitor, stats for <N> seconds" << std::endl;
    std::cout << "                 -h : print this text" << std::endl;
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

    char option = 0;
    uint32_t radioAddress = 0;

    while ((option = getopt(argc, argv, "K:N:n:h")) != -1) {
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
            readCurrentAndVoltage(mon, mqtt_client, atoi(optarg));
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

    parseOpt(argc, argv, mon, cryptoHandler);

    readerThread.join();

    return 0;
}
