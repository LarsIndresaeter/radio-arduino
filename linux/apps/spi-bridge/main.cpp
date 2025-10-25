
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
    std::cout << "raduino-spi-bridge" << std::endl;
    std::cout << "      -o : offset SPI register" << std::endl;
    std::cout << "      -r : SPI read <length> bytes" << std::endl;
    std::cout << "      -w : SPI write command" << std::endl;
    std::cout << "      -h : print this text" << std::endl;
}

void parseOpt(int argc, char* argv[], monitor& mon, LinuxCryptoHandler& cryptoHandler)
{
    char option = 0;
    uint8_t spiRegister = 0;

    while ((option = getopt(argc, argv, "o:r:w:h")) != -1) {
        switch (option) {
        case 'o':
            spiRegister = atoi(optarg);
            break;
        case 'w': {
            std::string s(optarg);
            std::vector<uint8_t> vec(s.begin(), s.end());
            uint8_t length = vec.size();
            if (length > 32) {
                length = 32;
            }
            std::cout << mon.get<>(RaduinoCommandSpiWrite(spiRegister, length, vec)) << std::endl;
        } break;
        case 'r': {
            uint8_t length = atoi(optarg);
            std::cout << mon.get<>(RaduinoCommandSpiRead(spiRegister, length)) << std::endl;
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
