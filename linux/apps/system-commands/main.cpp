
#include <chrono>
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

using namespace std::chrono_literals;

void print_usage()
{
    std::cout << "raduino-system-commands" << std::endl;
    std::cout << "       -K : encrypt command with transport key" << std::endl;
    std::cout << "       -N : wakeup node address" << std::endl;
    std::cout << "       -c : SHA1 command on gateway" << std::endl;
    std::cout << "       -C : SHA1 command on node" << std::endl;
    std::cout << "       -d : debug command on gateway" << std::endl;
    std::cout << "       -D : debug command on node" << std::endl;
    std::cout << "       -p : ping gateway" << std::endl;
    std::cout << "       -P : ping node" << std::endl;
    std::cout << "       -f : set data encryption key on gateway" << std::endl;
    std::cout << "       -F : set data encryption key on node" << std::endl;
    std::cout << "       -g : set transport encryption key on gateway" << std::endl;
    std::cout << "       -G : set transport encryption key on node" << std::endl;
    std::cout << "       -r : get random bytes command on gateway" << std::endl;
    std::cout << "       -R : get random bytes command on node" << std::endl;
    std::cout << "       -s : get statistics from gateway" << std::endl;
    std::cout << "       -S : get statistics from node" << std::endl;
    std::cout << "       -t : set transport encryption required (command must be encrypted) on gateway" << std::endl;
    std::cout << "       -T : set transport encryption required (command must be encrypted) on node" << std::endl;
    std::cout << "       -u : unencrypted session (command must be encrypted)" << std::endl;
    std::cout << "       -v : get device version from gateway" << std::endl;
    std::cout << "       -V : get device version from node" << std::endl;
    std::cout << "       -x : HOTP command on gateway" << std::endl;
    std::cout << "       -X : HOTP command on node" << std::endl;
    std::cout << "       -h : print this text" << std::endl;
}

void parseOpt(int argc, char* argv[], monitor& mon, LinuxCryptoHandler& cryptoHandler)
{
    char option = 0;
    uint8_t radioAddress = 0;

    while ((option = getopt(argc, argv, "K:N:c:C:f:F:g:G:xXrRdDpPt:T:usSUvVzZh")) != -1) {
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

        case 'c': {
            std::string s(optarg);
            std::cout << mon.get<>(RaduinoCommandSha1(s)) << std::endl;
        } break;
        case 'C': {
            std::string s(optarg);
            std::cout << mon.getRadio<>(RaduinoCommandSha1(s)) << std::endl;
        } break;
        case 'f': {
            std::string s(optarg);
            mon.get<>(RaduinoCommandSetKey('D', s));
        } break;
        case 'F': {
            std::string s(optarg);
            mon.getRadio<>(RaduinoCommandSetKey('D', s));
        } break;
        case 'g': {
            std::string s(optarg);
            mon.get<>(RaduinoCommandSetKey('T', s));
        } break;
        case 'G': {
            std::string s(optarg);
            mon.getRadio<>(RaduinoCommandSetKey('T', s));
        } break;
        case 'x':
            std::cout << mon.get<>(RaduinoCommandHotp()) << std::endl;
            break;
        case 'X':
            std::cout << mon.getRadio<>(RaduinoCommandHotp()) << std::endl;
            break;
        case 'r':
            std::cout << mon.get<>(RaduinoCommandRandom()) << std::endl;
            break;
        case 'R':
            std::cout << mon.getRadio<>(RaduinoCommandRandom()) << std::endl;
            break;
        case 'd':
            std::cout << mon.get<>(RaduinoCommandDebug()) << std::endl;
            break;
        case 'D':
            std::cout << mon.getRadio<>(RaduinoCommandDebug()) << std::endl;
            break;
        case 'p':
            std::cout << mon.get<>(RaduinoCommandPing()) << std::endl;
            break;
        case 'P':
            std::cout << mon.getRadio<>(RaduinoCommandPing()) << std::endl;
            break;
        case 't': {
            uint8_t flag = atoi(optarg);
            std::cout << mon.get<>(RaduinoCommandRequireTransportEncryption(flag)) << std::endl;
        } break;
        case 'T': {
            uint8_t flag = atoi(optarg);
            std::cout << mon.getRadio<>(RaduinoCommandRequireTransportEncryption(flag)) << std::endl;
        } break;
        case 'u':
            std::cout << mon.get<>(RaduinoCommandUnlockSession()) << std::endl;
            break;
        case 'U':
            std::cout << mon.getRadio<>(RaduinoCommandUnlockSession()) << std::endl;
            break;
        case 'v':
            std::cout << mon.get<>(RaduinoCommandGetVersion()) << std::endl;
            break;
        case 'V':
            std::cout << mon.getRadio<>(RaduinoCommandGetVersion()) << std::endl;
            break;
        case 's':
            std::cout << mon.get<>(RaduinoCommandGetStatistics()) << std::endl;
            break;
        case 'S':
            std::cout << mon.getRadio<>(RaduinoCommandGetStatistics()) << std::endl;
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
