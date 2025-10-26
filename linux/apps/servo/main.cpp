
#include <chrono>
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
    std::cout << "raduino-servo" << std::endl;
    std::cout << "          -g : get pulse width from servo controller 100-200 (1-2 ms)" << std::endl;
    std::cout << "  -s <value> : set pulse width 100-200 (1-2 ms)" << std::endl;
    std::cout << "          -V : Verbose on" << std::endl;
    std::cout << "          -h : print this text" << std::endl;
}

void parseOpt(int argc, char* argv[], monitor& mon, LinuxCryptoHandler& cryptoHandler)
{
    char option = 0;

    while ((option = getopt(argc, argv, "gs:Vh")) != -1) {
        switch (option) {
        case 'g':
            std::cout << mon.get<>(RaduinoCommandTimer()) << std::endl;
            break;
        case 's': {
            uint8_t value = atoi(optarg);
            std::cout << mon.get<>(RaduinoCommandPwm('b', 2, value)) << std::endl;
        } break;
        case 'V':
            mon.printDebug(true);
            mon.setPrintResponseTime(true);
            break;
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
