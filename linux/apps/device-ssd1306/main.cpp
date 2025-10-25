
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
    std::cout << "raduino-device-ws2812b" << std::endl;
    std::cout << "       -l : write text on ssd1306 LCD on gateway" << std::endl;
    std::cout << "       -L : write text on ssd1306 LCD on node" << std::endl;
    std::cout << "       -n : wakeup node address" << std::endl;
    std::cout << "       -h : print this text" << std::endl;
}

void parseOpt(int argc, char* argv[], monitor& mon, LinuxCryptoHandler& cryptoHandler)
{
    char option = 0;
    uint8_t spiRegister = 0;
    uint8_t radioAddress = 0;

    while ((option = getopt(argc, argv, "l:L:n:h")) != -1) {
        switch (option) {
        case 'l': {
            std::string s(optarg);
            mon.get<>(RaduinoCommandSsd1306(2, s)); // second line
        } break;
        case 'n':
            radioAddress = atoi(optarg);
            break;
        case 'L': {
            std::string s(optarg);
            RadioSession radioSession(mon, radioAddress);
            radioSession.wakeupNotResponding();
            std::cout << mon.getRadio<>(RaduinoCommandSsd1306(2, s)) << std::endl;
        } break;
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
