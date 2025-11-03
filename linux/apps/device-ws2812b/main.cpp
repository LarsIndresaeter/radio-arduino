
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
    std::cout << "       -K : encrypt command with transport key" << std::endl;
    std::cout << "       -N : wakeup node address" << std::endl;
    std::cout << "       -V : Verbose on" << std::endl;
    std::cout << "       -w : WS2812B <string>" << std::endl;
    std::cout << "       -h : print this text" << std::endl;
}

void parseOpt(int argc, char* argv[], monitor& mon, LinuxCryptoHandler& cryptoHandler)
{
    char option = 0;
    uint8_t spiRegister = 0;
    uint8_t radioAddress = 0;

    std::vector<uint8_t> red(8);
    std::vector<uint8_t> green(8);
    std::vector<uint8_t> blue(8);

    COMMANDS::WS2812B::command_t command;

    using namespace std::chrono;

    while ((option = getopt(argc, argv, "K:N:Vw:ah")) != -1) {
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
        case 'w': {
            std::string s(optarg);

            red.at(1) = 8;
            green.at(1) = 8;
            blue.at(1) = 8;
            std::cout << mon.get<>(RaduinoCommandWs2812b(red, green, blue)) << std::endl;

        } break;
        case 'a': {
            for (int k = 0; k < 19; k++) {
                for (int i = 0; i < sizeof(command.red); i++) {
                    if (k % 3 == 0) {
                        red.at(i) = 3;
                        green.at(i) = 0;
                        blue.at(i) = 0;
                    }

                    if (k % 3 == 1) {
                        red.at(i) = 0;
                        green.at(i) = 3;
                        blue.at(i) = 0;
                    }

                    if (k % 3 == 2) {
                        red.at(i) = 0;
                        green.at(i) = 0;
                        blue.at(i) = 3;
                    }

                    mon.get<>(RaduinoCommandWs2812b(red, green, blue));
                    std::this_thread::sleep_for(100ms);
                }
            }
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
