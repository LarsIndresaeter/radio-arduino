
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
    std::cout << "       -B : Blink command" << std::endl;
    std::cout << "       -S : SHA1 command " << std::endl;
    std::cout << "       -H : HOTP command" << std::endl;
    std::cout << "       -P : pwm command" << std::endl;
    std::cout << "       -R : get random bytes command" << std::endl;
    std::cout << "       -C : print counter values" << std::endl;
    std::cout << "       -D : debug command" << std::endl;
    std::cout << "       -t : disable transport encryption" << std::endl;
    std::cout << "       -T : enable transport encryption" << std::endl;
    std::cout << "       -Z : set device name" << std::endl;
    std::cout << "       -z : get device name" << std::endl;
    std::cout << "       -a : get device version" << std::endl;
    std::cout << "       -x : get statistics" << std::endl;
    std::cout << "       -j : read vcc" << std::endl;
    std::cout << "       -s : sleep" << std::endl;
    std::cout << "       -L : print text on LCD" << std::endl;
    std::cout << "       -p : ping command" << std::endl;
    std::cout << "       -E : set encryption key" << std::endl;
    std::cout << "       -K : set transport key on device (command must be encrypted)" << std::endl;
    std::cout << "       -b : use transport key" << std::endl;
    std::cout << "       -r : set transport encryption required (command must be encrypted)" << std::endl;
    std::cout << "       -n : reboot gateway as node" << std::endl;
    std::cout << "       -u : unencrypted session (command must be encrypted)" << std::endl;
    std::cout << "       -h : print this text" << std::endl;
}

void parseOpt(int argc, char* argv[], monitor& mon, LinuxCryptoHandler& cryptoHandler)
{
    char option = 0;
    uint16_t i2cDeviceOffset = 0;
    uint8_t i2cDeviceAddress = 0b10100000;

    while ((option = getopt(argc, argv, "P:DBSHCs:RVvhtTgGE:K:b:r:Z:zL:Jjpaxnu")) != -1) {
        switch (option) {
        case 's': {
            uint32_t delay = atoi(optarg);
            std::cout << mon.get<>(RaduinoCommandSleep(delay), static_cast<std::chrono::milliseconds>(delay + 2000))
                      << std::endl;
        } break;
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
        case 'B':
            std::cout << mon.get<>(RaduinoCommandBlink(), static_cast<std::chrono::milliseconds>(4000)) << std::endl;
            break;
        case 'L': {
            std::string s(optarg);
            mon.get<>(RaduinoCommandSsd1306(2, s)); // second line
        } break;
        case 'H':
            std::cout << mon.get<>(RaduinoCommandHotp()) << std::endl;
            break;
        case 'P': {
            uint8_t value = atoi(optarg);
            std::cout << mon.get<>(RaduinoCommandPwm('b', 2, value)) << std::endl;
        } break;
        case 'G':
            std::cout << mon.get<>(RaduinoCommandGpio()) << std::endl;
            break;
        case 'R':
            std::cout << mon.get<>(RaduinoCommandRandom()) << std::endl;
            break;
        case 'C':
            mon.printCounterValues();
            break;
        case 'D':
            std::cout << mon.get<>(RaduinoCommandDebug()) << std::endl;
            break;
        case 'p':
            std::cout << mon.get<>(RaduinoCommandPing()) << std::endl;
            break;
        case 'j':
            std::cout << mon.get<>(RaduinoCommandVcc()) << std::endl;
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
        case 'r': {
            uint8_t flag = atoi(optarg);
            std::cout << mon.get<>(RaduinoCommandRequireTransportEncryption(flag)) << std::endl;
        } break;
        case 'u':
            std::cout << mon.get<>(RaduinoCommandUnlockSession()) << std::endl;
            break;
        case 'Z': {
            std::string s(optarg);
            mon.get<>(RaduinoCommandSetDeviceName(s));
        } break;
        case 'z':
            std::cout << mon.get<>(RaduinoCommandGetDeviceName()) << std::endl;
            break;
        case 'a':
            std::cout << mon.get<>(RaduinoCommandGetVersion()) << std::endl;
            break;
        case 'x':
            std::cout << mon.get<>(RaduinoCommandGetStatistics()) << std::endl;
            break;
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
