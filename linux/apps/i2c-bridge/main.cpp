
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
    std::cout << "          -K : encrypt command with transport key" << std::endl;
    std::cout << "          -a : device address" << std::endl;
    std::cout << "          -c : control register address" << std::endl;
    std::cout << " -r <length> : read <length> bytes" << std::endl;
    std::cout << " -s <string> : write <string>" << std::endl;
    std::cout << "  -w <value> : write <value>" << std::endl;
    std::cout << "          -V : Verbose on" << std::endl;
    std::cout << "          -h : print this text" << std::endl;
}

void parseOpt(int argc, char* argv[], monitor& mon, LinuxCryptoHandler& cryptoHandler)
{
    char option = 0;
    uint8_t i2cDeviceAddress = 0b10100000;

    bool readOperation = false;
    bool writeOperation = false;
    uint16_t registerAddress = 0;
    uint8_t length = 0;
    std::string writeString = "";
    uint16_t writeValue = 0;

    while ((option = getopt(argc, argv, "K:a:c:r:s:w:Vh")) != -1) {
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

        case 'a':
            i2cDeviceAddress = atoi(optarg);
            break;
        case 'c':
            registerAddress = atoi(optarg);
            break;
        case 'r':
            length = atoi(optarg);
            readOperation = true;
            break;
        case 's':
            writeString = optarg;
            writeOperation = true;
            break;
        case 'w':
            writeValue = atoi(optarg);
            writeOperation = true;
            break;
        case 'V':
            mon.printDebug(true);
            mon.setPrintResponseTime(true);
            break;
        case 'h':
            print_usage();
            break;
        }
    }

    if (readOperation && writeOperation) {
        std::cout << "you can only read or write, not both. exit" << std::endl;
        exit(1);
    }

    if (readOperation) {
        if(length == 2)
        {
            auto result = mon.get<>(RaduinoCommandI2cRead(i2cDeviceAddress, registerAddress, length));
            uint16_t value = result.responseStruct().data[0] << 8;
            value |= result.responseStruct().data[1];

            //std::cout << "register[" << std::to_string(registerAddress) << "]=" << std::to_string(value) << std::endl;
            std::cout << "register[" << std::dec << registerAddress << "]=" << value << ", 0x" << std::hex << value << std::endl;
        } else{
            std::cout << mon.get<>(RaduinoCommandI2cRead(i2cDeviceAddress, registerAddress, length)) << std::endl;
        }
    }
    else if (writeOperation) {
        if (writeString == "") {
            std::vector<uint8_t> data_vector(2);
            data_vector.at(0) = writeValue >> 8;
            data_vector.at(1) = writeValue;
            std::cout << mon.get<>(RaduinoCommandI2cWrite(i2cDeviceAddress, registerAddress, 2, data_vector))
                      << std::endl;
        }
        else {
            std::vector<uint8_t> data_vector;
            data_vector.assign(writeString.begin(), writeString.end());
            length = data_vector.size();
            if (length > 16) {
                length = 16;
            }
            std::cout << mon.get<>(RaduinoCommandI2cWrite(i2cDeviceAddress, registerAddress, length, data_vector))
                      << std::endl;
        }
    }
    else {
        std::cout << "device not specified. exit" << std::endl;
        exit(1);
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
