
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

using namespace std::chrono_literals;

void print_usage()
{
    std::cout << "raduino-gateway" << std::endl;
    std::cout << "       -V : Verbose on" << std::endl;
    std::cout << "       -v : Verbose off" << std::endl;
    std::cout << "       -S : SHA1 command " << std::endl;
    std::cout << "       -C : print counter values" << std::endl;
    std::cout << "       -t : disable transport encryption" << std::endl;
    std::cout << "       -T : enable transport encryption" << std::endl;
    std::cout << "       -E : set encryption key" << std::endl;
    std::cout << "       -K : set transport key on device (command must be encrypted)" << std::endl;
    std::cout << "       -b : use transport key" << std::endl;
    std::cout << "       -n : reboot gateway as node" << std::endl;
    std::cout << "       -h : print this text" << std::endl;
}

void parseOpt(int argc, char* argv[], monitor& mon, LinuxCryptoHandler& cryptoHandler)
{
    char option = 0;
    uint16_t i2cDeviceOffset = 0;
    uint8_t i2cDeviceAddress = 0b10100000;

    while ((option = getopt(argc, argv, "SCVvhtTgGE:K:b:Jn")) != -1) {
        switch (option) {
        case 'V':
            mon.printDebug(true);
            mon.setPrintResponseTime(true);
            break;
        case 't':
            mon.setTransportEncryption(false);
            break;
        case 'T':
            mon.setTransportEncryption(true);
            break;
        case 'v':
            mon.printDebug(false);
            mon.setPrintResponseTime(false);
            break;
        case 'C':
            mon.printCounterValues();
            break;
        case 'E': {
            std::string s(optarg);
            mon.get<>(RaduinoCommandSetKey('E', s));
        } break;
        case 'K': {
            std::string s(optarg);
            mon.get<>(RaduinoCommandSetKey('T', s));
        } break;
        case 'b': {
            std::string s(optarg);
            std::vector<uint8_t> key(16, 0);

            // read key ascii values
            for (uint8_t i = 0; i < s.size() && i < 16; i++) {
                key.at(i) = s.at(i);
            }

            // set key
            cryptoHandler.setTransportKey((uint8_t*)&key[0]);
            cryptoHandler.setMacKey((uint8_t*)&key[0]);
        } break;
        case 'n':
            std::cout << mon.get<>(RaduinoCommandSetRadioRole('n')) << std::endl;
            std::cout << mon.get<>(RaduinoCommandSoftReset()) << std::endl;
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
