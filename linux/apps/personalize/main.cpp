#include <chrono>
#include <cmath>
#include <cmd/commands.hxx>
#include <eventprocess.hpp>
#include <filesystem>
#include <linuxCryptoHandler.hpp>
#include <monitor.hpp>
#include <numeric>
#include <thread>
#include <uart.hpp>

void print_usage()
{
    std::cout << "raduino-node" << std::endl;
    std::cout << "       -n : set device name" << std::endl;
    std::cout << "       -c : current transport key" << std::endl;
    std::cout << "       -t : new transport key" << std::endl;
    std::cout << "       -e : new encryption key" << std::endl;
    std::cout << "       -d : dump eeprom contents" << std::endl;
    std::cout << "       -r : set radio role <gateway|node>" << std::endl;
    std::cout << "       -s : set requireTransportEncyption flag <0|1>" << std::endl;
    std::cout << "       -h : print this text" << std::endl;
}

void dumpEeprom(monitor& mon, int length)
{
    std::cout << "dump eeprom" << std::endl;

    if (length > 64) {
        length = 64;
    }

    std::vector<uint8_t> eeprom;
    for (int i = 0; i < 16 * length; i++) {
        eeprom.push_back(
            mon.get<>(RaduinoCommandEepromRead(i)).responseStruct().data);
    }

    for (int i = 0; i < length; i++) {
        std::cout.fill('0');
        std::cout.width(4);
        std::cout << std::hex << static_cast<int>(i * 16) << " ";
        for (int j = 0; j < 16; j++) {
            std::cout.fill('0');
            std::cout.width(2);

            std::cout << std::hex
                      << static_cast<int>(eeprom.at(i * 16 + j));
            std::cout << " ";
        }
        std::cout << " : ";
        for (int j = 0; j < 16; j++) {
            std::cout << std::hex << std::uppercase
                      << eeprom.at(i * 16 + j) << " ";
        }
        std::cout << std::endl;
    }
}

void setCurrentEncryptionKey(monitor& mon, LinuxCryptoHandler& cryptoHandler, std::string currentTransportKey)
{
    std::vector<uint8_t> key(16, 0);
    for (uint8_t i = 0; i < currentTransportKey.size() && i < 16; i++) {
        key.at(i) = currentTransportKey.at(i);
    }

    cryptoHandler.setTransportKey((uint8_t*)&key[0]);
    cryptoHandler.setMacKey((uint8_t*)&key[0]);
}

void pingAndExitIfNoResponse(monitor& mon)
{
    bool deviceResponded = false;
    for (int i = 0; i < 10; i++) {
        mon.get<>(RaduinoCommandPing());

        if (mon.lastCommandReturnedValidResponse()) {
            deviceResponded = true;
        }
    }

    if (false == deviceResponded) {
        std::cout << "device not responding. exit!" << std::endl;
        exit(1);
    }
}

void parseOpt(int argc, char* argv[], monitor& mon, LinuxCryptoHandler& cryptoHandler)
{
    char option = 0;
    bool verbose = false;
    std::string setNewDeviceName = "no-name";
    std::string setNewTransportKey = "transport-key";
    std::string setNewEncryptionKey = "secret";
    std::string currentTransportKey = "";
    uint8_t set_transport_encryption_required = 0;
    uint8_t setRadioRole = 'g';
    uint8_t setRequireTransportEncryption = 0;
    bool dump_eeprom = false;
    bool name_option_present = false;

    while ((option
               = getopt(argc, argv, "n:c:t:e:r:ds:h"))
        != -1) {
        switch (option) {
        case 'n':
            setNewDeviceName = optarg;
            name_option_present = true;
            break;
        case 'c':
            currentTransportKey = optarg;
            break;
        case 't':
            setNewTransportKey = optarg;
            break;
        case 'e':
            setNewEncryptionKey = optarg;
            break;
        case 's':
            setRequireTransportEncryption = atoi(optarg);
            break;
        case 'd':
            dump_eeprom = true;
            break;
        case 'r': {
            std::string role(optarg);

            if (role == "gateway") {
                setRadioRole = 'g';
            }
            else if (role == "node") {
                setRadioRole = 'n';
            }
            else {
                std::cout << "role '" + role + "' is not a valid option. Use 'gateway' or 'node'" << std::endl;
                exit(1);
            }
        } break;
        case 'h':
            print_usage();
            exit(0);
            break;
        }
    }

    if (false == name_option_present) {
        std::cout << "no name supplied. exit" << std::endl;
        print_usage();
        exit(1);
    }

    setCurrentEncryptionKey(mon, cryptoHandler, currentTransportKey);
    pingAndExitIfNoResponse(mon);

    if (dump_eeprom) {
        dumpEeprom(mon, 10);
    }

    mon.setTransportEncryption(true);
    std::cout << mon.get<>(RaduinoCommandSetDeviceName(setNewDeviceName)) << std::endl;
    std::cout << mon.get<>(RaduinoCommandSetKey('E', setNewEncryptionKey)) << std::endl;
    std::cout << mon.get<>(RaduinoCommandSetKey('T', setNewTransportKey)) << std::endl;
    std::cout << mon.get<>(RaduinoCommandRequireTransportEncryption(setRequireTransportEncryption)) << std::endl;
    std::cout << mon.get<>(RaduinoCommandSetRadioRole(setRadioRole)) << std::endl;

    if (dump_eeprom) {
        dumpEeprom(mon, 10);
    }

    std::cout << "reset device" << std::endl;
    mon.get<>(RaduinoCommandSoftReset());

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
