
#ifdef USE_FEATURE_MQTT
#include "mqtt/async_client.h"
#endif

#include <chrono>
#include <cmath>
#include <cmd/commands.hxx>
#include <eventprocess.hpp>
#include <filesystem>
#include <linuxCryptoHandler.hpp>
#include <monitor.hpp>
#include <numeric>
#include <radioSession.hpp>
#include <thread>
#include <uart.hpp>

void print_usage()
{
    std::cout << "raduino-node" << std::endl;
    std::cout << "       -K : encrypt command with transport key" << std::endl;
    std::cout << "       -V : Verbose on" << std::endl;
    std::cout << "       -v : Verbose off" << std::endl;
    std::cout << "       -C : print counter values" << std::endl;
    std::cout << "       -t : disable transport encryption" << std::endl;
    std::cout << "       -T : enable transport encryption" << std::endl;
    std::cout << "       -g : reboot node as gateway" << std::endl;
    std::cout << "       -w : wake up sleeping rx node" << std::endl;
    std::cout << "       -n : wakeup node address" << std::endl;
    std::cout << "       -a : update node address" << std::endl;
    std::cout << "       -k : set keep alive interval" << std::endl;
    std::cout << "       -h : print this text" << std::endl;
}

void parseOpt(int argc, char* argv[], monitor& mon, LinuxCryptoHandler& cryptoHandler)
{
    char option = 0;
    uint8_t radioAddress = 0;
    uint8_t keepAliveInterval = 0;
    bool verbose = false;

    while ((option = getopt(argc, argv, "CVvtTgNwqjn:a:k:K:h")) != -1) {
        switch (option) {
        case 'V':
            verbose = true;
            mon.printDebug(true);
            mon.setPrintResponseTime(true);
            break;
        case 't':
            mon.setTransportEncryption(false);
            break;
        case 'T':
            mon.setTransportEncryption(true);
            break;
        case 'w':
            std::cout << mon.get<>(RaduinoCommandWakeup(false), static_cast<std::chrono::milliseconds>(12000))
                      << std::endl;
            break;
        case 'v':
            mon.printDebug(false);
            verbose = false;
            mon.setPrintResponseTime(false);
            break;
        case 'g': {
            std::cout << mon.getRadio<>(RaduinoCommandSetRadioRole('g')) << std::endl;
            std::cout << mon.getRadio<>(RaduinoCommandSoftReset()) << std::endl;
        } break;
        case 'C':
            mon.printCounterValues();
            break;
        case 'h':
            print_usage();
            break;
        case 'n':
            radioAddress = atoi(optarg);
            {
                RadioSession radioSession(mon, radioAddress);
                radioSession.setKeepAliveInterval(keepAliveInterval);
                if (verbose) {
                    radioSession.setVerbose(true);
                }
                radioSession.wakeupNotResponding();
            }
            break;
        case 'a':
            std::cout << mon.getRadio<>(RaduinoCommandSetNodeAddress(atoi(optarg))) << std::endl;
            break;
        case 'k':
            keepAliveInterval = atoi(optarg);
            break;
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

    parseOpt(argc, argv, mon, cryptoHandler);

    readerThread.join();

    return 0;
}
