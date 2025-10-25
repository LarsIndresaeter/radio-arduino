
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
    std::cout << "raduino-device-ws2812b" << std::endl;
    std::cout << "       -W : WS2812B <string>" << std::endl;
    std::cout << "       -F : Read servo PWM signal and display on ws2812b ring" << std::endl;
    std::cout << "       -h : print this text" << std::endl;
}

void parseOpt(int argc, char* argv[], monitor& mon, LinuxCryptoHandler& cryptoHandler)
{
    char option = 0;
    uint8_t spiRegister = 0;

    while ((option = getopt(argc, argv, "W:Fh")) != -1) {
        switch (option) {
        case 'W': {
            std::vector<uint8_t> red(45);
            std::vector<uint8_t> green(45);
            std::vector<uint8_t> blue(45);
            COMMANDS::WS2812B::command_t command;
            std::string s(optarg);

            if (s.at(0) == 'a') {
                using namespace std::chrono;
                int random_variable
                    = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count() % 90 + 45;
                for (int k = 0; k < random_variable; k++) {
                    for (int i = 0; i < sizeof(command.red); i++) {
                        if (i % 9 < 3) {
                            red.at(i) = 1;
                        }
                        else if (i % 9 > 5) {
                            green.at(i) = 1;
                        }
                        else {
                            blue.at(i) = 1;
                        }
                    }

                    red.at(k) = 8;
                    green.at(k) = 8;
                    blue.at(k) = 8;
                    RaduinoCommandWs2812b ws2812b(red, green, blue);
                    mon.get<>(ws2812b);
                    std::this_thread::sleep_for((k + 5) * 1ms);
                }
            }

        } break;
        case 'F': {
            while (true) {
                auto r = mon.get<>(RaduinoCommandTimer());

                uint16_t pulse_width = r.responseStruct().getPulsewidth();

                std::cout << "pulse_width=" << pulse_width << std::endl;

                if ((pulse_width > 1000) && (pulse_width < 2000)) {
                    std::vector<uint8_t> red(45);
                    std::vector<uint8_t> green(45);
                    std::vector<uint8_t> blue(45);

                    int v = (pulse_width - 1000) / 22;
                    for (int i = 0; i < v; i++) {
                        red.at(i) = 1;
                        green.at(i) = 1;
                        blue.at(i) = 1;
                    }

                    RaduinoCommandWs2812b ws2812b(red, green, blue);
                    mon.get<>(ws2812b);
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
