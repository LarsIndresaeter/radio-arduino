
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
    std::cout << "raduino-i2c-bridge" << std::endl;
    std::cout << "       -w : I2C write command gateway" << std::endl;
    std::cout << "       -W : I2C write command node" << std::endl;
    std::cout << "       -r : I2C read command gateway" << std::endl;
    std::cout << "       -R : I2C read command node" << std::endl;
    std::cout << "       -d : I2C device address" << std::endl;
    std::cout << "       -o : I2C device offset" << std::endl;
    std::cout << "       -h : print this text" << std::endl;
}

void parseOpt(int argc, char* argv[], monitor& mon, LinuxCryptoHandler& cryptoHandler)
{
    char option = 0;
    uint16_t i2cDeviceOffset = 0;
    uint8_t i2cDeviceAddress = 0b10100000;

    while ((option = getopt(argc, argv, "w:W:r:R:d:o:h")) != -1) {
        switch (option) {
        case 'd':
            i2cDeviceAddress = atoi(optarg);
            break;
        case 'o':
            i2cDeviceOffset = atoi(optarg);
            break;
        case 'i': {
            std::string s(optarg);
            std::vector<uint8_t> vec(s.begin(), s.end());
            std::cout << mon.get<>(RaduinoCommandI2cWrite(i2cDeviceAddress, i2cDeviceOffset, vec.size(), vec))
                      << std::endl;
        } break;
        case 'I': {
            std::string s(optarg);
            std::vector<uint8_t> vec(s.begin(), s.end());
            std::cout << mon.getRadio<>(RaduinoCommandI2cWrite(i2cDeviceAddress, i2cDeviceOffset, vec.size(), vec))
                      << std::endl;
        } break;

        case 'r':
            std::cout << mon.get<>(RaduinoCommandI2cRead(i2cDeviceAddress, i2cDeviceOffset, atoi(optarg))) << std::endl;
            break;
        case 'R':
            std::cout << mon.getRadio<>(RaduinoCommandI2cRead(i2cDeviceAddress, i2cDeviceOffset, atoi(optarg))) << std::endl;
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
