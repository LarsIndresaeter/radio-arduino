
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
    std::cout << "raduino-device-lsm303d" << std::endl;
    std::cout << "           -K <key> : encrypt command with transport key" << std::endl;
    std::cout << "       -N <address> : wakeup node address" << std::endl;
    std::cout << "                 -V : Verbose on" << std::endl;
    std::cout << "                 -r : read lsm303d from gateway" << std::endl;
    std::cout << "                 -R : read lsm303d from node" << std::endl;
    std::cout << "                 -h : print this text" << std::endl;
}

void parseOpt(int argc, char* argv[], monitor& mon, LinuxCryptoHandler& cryptoHandler)
{
    char option = 0;
    uint8_t spiRegister = 0;
    uint8_t radioAddress = 0;

    COMMANDS::GET_LSM303D::command_t command;

    using namespace std::chrono;

    while ((option = getopt(argc, argv, "K:N:VrRh")) != -1) {
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

        case 'V':
            mon.printDebug(true);
            mon.setPrintResponseTime(true);
            break;
        case 'r': {
            COMMANDS::GET_LSM303D::response_t result = mon.get<>(RaduinoCommandGetLsm303d()).responseStruct();
            std::cout << "accelerometer=[";
            std::cout << std::to_string((int16_t)result.getAccelerometerx()) << ", ";
            std::cout << std::to_string((int16_t)result.getAccelerometery()) << ", ";
            std::cout << std::to_string((int16_t)result.getAccelerometerz()) << "]";
            std::cout << std::endl;
        } break;
        case 'R': {
            COMMANDS::GET_LSM303D::response_t result = mon.getRadio<>(RaduinoCommandGetLsm303d()).responseStruct();
            std::cout << "accelerometer=[";
            std::cout << std::to_string((int16_t)result.getAccelerometerx()) << ", ";
            std::cout << std::to_string((int16_t)result.getAccelerometery()) << ", ";
            std::cout << std::to_string((int16_t)result.getAccelerometerz()) << "]";
            std::cout << std::endl;
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
