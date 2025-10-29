
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
#include <thread>
#include <uart.hpp>

void print_usage()
{
    std::cout << "raduino-device-nrf24l01" << std::endl;
    std::cout << "      -r : set device in serial bridge as reader" << std::endl;
    std::cout << "      -s : set device in serial bridge as sender" << std::endl;
    std::cout << "      -h : print this text" << std::endl;
}

void parseOpt(int argc, char* argv[], monitor& mon, LinuxCryptoHandler& cryptoHandler)
{
    char option = 0;
    uint8_t spiRegister = 0;

    while ((option = getopt(argc, argv, "r:s:h")) != -1) {
        switch (option) {
        case 's': {
            std::string s(optarg);
            std::vector<uint8_t> address = { 0xF0, 0xF0, 0xF0, 0xF0, 0xC2 };
            mon.get<>(RaduinoCommandNrf24l01Init(address, address, 121, true));
            std::cout << mon.get<>(RaduinoCommandRadioUart(s.at(0))) << std::endl;
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
