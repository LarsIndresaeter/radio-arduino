
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
    std::cout << "raduino-device-atmega328" << std::endl;
    std::cout << "       -b : Blink command to gateway" << std::endl;
    std::cout << "       -B : Blink command to node" << std::endl;
    std::cout << "       -g : get gpio values on gateway" << std::endl;
    std::cout << "       -G : get gpio values on node" << std::endl;
    std::cout << "       -j : read vcc from gateway" << std::endl;
    std::cout << "       -J : read vcc from node" << std::endl;
    std::cout << "       -p : pwm command to gateway" << std::endl;
    std::cout << "       -P : pwm command to node" << std::endl;
    std::cout << "       -q : read quadrature encoder from gateway" << std::endl;
    std::cout << "       -Q : read quadrature encoder from node" << std::endl;
    std::cout << "       -C : wake up sleeping rx node if data available flag is set" << std::endl;
    std::cout << "       -A : read quadrature encoder from node on change" << std::endl;
    std::cout << "       -y : set device name from gateway" << std::endl;
    std::cout << "       -Y : set device name from node" << std::endl;
    std::cout << "       -z : get device name from gateway" << std::endl;
    std::cout << "       -Z : get device name from node" << std::endl;
    std::cout << "       -s : sleep gateway" << std::endl;
    std::cout << "       -S : sleep node" << std::endl;
    std::cout << "       -t : get pulse width from servo controller 100-200 (1-2 ms) on gateway" << std::endl;
    std::cout << "       -T : get pulse width from servo controller 100-200 (1-2 ms) on node" << std::endl;
    std::cout << "      -h : print this text" << std::endl;
}

void parseOpt(int argc, char* argv[], monitor& mon, LinuxCryptoHandler& cryptoHandler)
{
    char option = 0;
    uint8_t spiRegister = 0;

    while ((option = getopt(argc, argv, "AbBCgGjJy:p:P:qQY:zZs:S:tTh")) != -1) {
        switch (option) {
        case 'b':
            std::cout << mon.get<>(RaduinoCommandBlink(), static_cast<std::chrono::milliseconds>(4000)) << std::endl;
            break;
        case 'B':
            std::cout << mon.getRadio<>(RaduinoCommandBlink(), static_cast<std::chrono::milliseconds>(4000)) << std::endl;
            break;
        case 'g':
            std::cout << mon.get<>(RaduinoCommandGpio()) << std::endl;
            break;
        case 'G':
            std::cout << mon.getRadio<>(RaduinoCommandGpio()) << std::endl;
            break;
        case 'j':
            std::cout << mon.get<>(RaduinoCommandVcc()) << std::endl;
            break;
        case 'J':
            std::cout << mon.getRadio<>(RaduinoCommandVcc()) << std::endl;
            break;
        case 'p': {
            uint8_t value = atoi(optarg);
            std::cout << mon.get<>(RaduinoCommandPwm('b', 2, value)) << std::endl;
        } break;
        case 'P': {
            uint8_t value = atoi(optarg);
            std::cout << mon.getRadio<>(RaduinoCommandPwm('b', 2, value)) << std::endl;
        } break;
        case 'q':
            std::cout << mon.get<>(RaduinoCommandQuadratureEncoder()) << std::endl;
            break;
        case 'Q':
            std::cout << mon.getRadio<>(RaduinoCommandQuadratureEncoder()) << std::endl;
            break;
        case 'C':
            std::cout << mon.get<>(RaduinoCommandWakeup(true), static_cast<std::chrono::milliseconds>(12000))
                      << std::endl;
            break;
        case 'A': {
            while (1) {
                mon.get<>(RaduinoCommandWakeup(true), static_cast<std::chrono::milliseconds>(12000));
                if (mon.lastCommandReturnedValidResponse()) {
                    std::cout << mon.getRadio<>(RaduinoCommandQuadratureEncoder()) << std::endl;
                }
            }
        } break;
        case 'y': {
            std::string s(optarg);
            mon.get<>(RaduinoCommandSetDeviceName(s));
        } break;
        case 'Y': {
            std::string s(optarg);
            mon.getRadio<>(RaduinoCommandSetDeviceName(s));
        } break;
        case 'z':
            std::cout << mon.get<>(RaduinoCommandGetDeviceName()) << std::endl;
            break;
        case 'Z':
            std::cout << mon.getRadio<>(RaduinoCommandGetDeviceName()) << std::endl;
            break;
        case 's': {
            uint32_t delay = atoi(optarg);
            std::cout << mon.get<>(RaduinoCommandSleep(delay), static_cast<std::chrono::milliseconds>(delay + 2000))
                      << std::endl;
        } break;
        case 'S': {
            uint32_t delay = atoi(optarg);
            std::cout << mon.getRadio<>(RaduinoCommandSleep(delay), static_cast<std::chrono::milliseconds>(delay + 2000))
                      << std::endl;
        } break;
        case 't':
            std::cout << mon.get<>(RaduinoCommandTimer()) << std::endl;
            break;
        case 'T':
            std::cout << mon.getRadio<>(RaduinoCommandTimer()) << std::endl;
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
