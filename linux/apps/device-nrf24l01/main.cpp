
#include <chrono>
#include <cinttypes>
#include <cmath>
#include <cmd/commands.hxx>
#include <crypto.hpp>
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
    std::cout << "raduino-device-nrf24l01" << std::endl;
    std::cout << "           -K <key> : encrypt command with transport key" << std::endl;
    std::cout << "       -N <address> : wakeup node address" << std::endl;
    std::cout <<           "       -i : initialize nrf24l01 on gateway" << std::endl;
    std::cout << "                 -r : set device in serial bridge as reader" << std::endl;
    std::cout << "                 -s : set device in serial bridge as sender" << std::endl;
    std::cout << "                 -h : print this text" << std::endl;
}

void parseOpt(int argc, char* argv[], monitor& mon, LinuxCryptoHandler& cryptoHandler)
{
    char option = 0;
    uint8_t spiRegister = 0;
    uint8_t radioAddress = 0;

    while ((option = getopt(argc, argv, "K:N:ir:s:h")) != -1) {
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

        case 's': {
            std::string s(optarg);
            std::vector<uint8_t> address = { 0xF0, 0xF0, 0xF0, 0xF0, 0xC2 };
            mon.get<>(RaduinoCommandNrf24l01Init(address, address, 121, true));
            std::cout << mon.get<>(RaduinoCommandRadioUart(s.at(0))) << std::endl;
        } break;
        case 'i': {
            std::string s(optarg);
            std::vector<uint8_t> address = { 0xF0, 0xF0, 0xF0, 0xF0, 0xC2 };
            mon.get<>(RaduinoCommandNrf24l01Init(address, address, 121, true));
        } break;
        case 'r': {
            {
                std::string s(optarg);
                std::vector<uint8_t> address = { 0xF0, 0xF0, 0xF0, 0xF0, 0xC2 };
                mon.get<>(RaduinoCommandNrf24l01Init(address, address, 121, false));
                std::cout << mon.get<>(RaduinoCommandRadioUart(s.at(0))) << std::endl;
            }
            break;
        }
        case 'h':
            print_usage();
            break;
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
