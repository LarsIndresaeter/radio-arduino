
#ifdef USE_FEATURE_MQTT
#include "mqtt/async_client.h"
#endif

#include <chrono>
#include <cmath>
#include <cmd/commands.hpp>
#include <crypto.hpp>
#include <eventprocess.hpp>
#include <filesystem>
#include <linuxCryptoHandler.hpp>
#include <monitor.hpp>
#include <numeric>
#include <thread>
#include <uart.hpp>
#include <radioSession.hpp>

using namespace std::chrono_literals;

void testDecryptOnAvr(monitor& mon)
{
    // std::cout << "encrypt on PC, decrypt on AVR" << std::endl;
    Crypto c {};
    Crypto::AesBlock key({ 's', 'e', 'c', 'r', 'e', 't' });
    c.setEncryptKey(key);

    Crypto::AesBlock plaintext({ 'T', 'e', 's', 't' });

    Crypto::AesBlock ciphertext = c.encrypt(plaintext);

    UartCommandAes decryptedInAvr
        = mon.get<>(UartCommandAes('d', c.getVector(ciphertext)));

    std::string responseString { reinterpret_cast<char*>(
        decryptedInAvr.responseStruct().data) };
    std::cout << "pt received   : " << responseString << std::endl;
}

void testEncryptOnAvr(monitor& mon)
{
    // std::cout << "encrypt on AVR, decrypt on PC" << std::endl;
    std::string s { "Test" };
    std::vector<uint8_t> v { s.begin(), s.end() };
    UartCommandAes encryptedInAvr = mon.get<>(UartCommandAes('c', v));

    Crypto c {};
    Crypto::AesBlock key({ 's', 'e', 'c', 'r', 'e', 't' });
    c.setDecryptKey(key);

    auto r = encryptedInAvr.responseStruct();
    Crypto::AesBlock ciphertext(r.data);

    Crypto::AesBlock plaintext = c.decrypt(ciphertext);

    std::cout << "ct response   : " << plaintext.getDataAsString() << std::endl;
}

void testAes(monitor& mon)
{
    testDecryptOnAvr(mon);
    testEncryptOnAvr(mon);
}

void print_usage()
{
    std::cout << "raduino-test" << std::endl;
    std::cout << "       -A : test AES" << std::endl;
    std::cout << "       -C : print counter values" << std::endl;
    std::cout << "       -E : EEPROM command" << std::endl;
    std::cout << "       -I : I2C read command" << std::endl;
    std::cout << "       -K : set AES Key" << std::endl;
    std::cout << "       -O : Test SPI commands" << std::endl;
    std::cout << "       -d : I2C device address" << std::endl;
    std::cout << "       -g : dump eeprom from mega328p" << std::endl;
    std::cout << "       -h : print this text" << std::endl;
    std::cout << "       -i : I2C write command" << std::endl;
    std::cout << "       -o : I2C device offset" << std::endl;
    std::cout << "       -b : test json formatter" << std::endl;
    std::cout << "       -S : rx and tx statistics for node and gateway" << std::endl;
}

void compareResult(uint8_t expected, uint8_t actual)
{
    if (expected != actual) {
        std::cout << std::endl;
        std::cout << "ERROR: invalid result! expected: "
                  << static_cast<int>(expected)
                  << " actual: " << static_cast<int>(actual) << std::endl;
        std::cout << std::endl;
    }
}

void parseOpt(int argc, char* argv[], monitor& mon)
{
    char option = 0;
    uint16_t i2cDeviceOffset = 0;
    uint8_t i2cDeviceAddress = 0b10100000;

    while ((option
            = getopt(argc, argv, "ACEI:K:O:d:ghi:o:bS"))
           != -1) {
        switch (option) {
        case 's':
            {
            uint32_t delay = atoi(optarg);
            std::cout << mon.getRadio<>(UartCommandSleep(delay), static_cast<std::chrono::milliseconds>(delay + 2000)) << std::endl;
            }
            break;
        case 'E':
            std::cout << mon.get<>(UartCommandEepromWrite(2, 3)) << std::endl;
            std::cout << mon.get<>(UartCommandEepromRead(2)) << std::endl;
            compareResult(
                3, mon.get<>(UartCommandEepromRead(2)).responseStruct().data);

            std::cout << mon.get<>(UartCommandEepromWrite(600, 3)) << std::endl;
            std::cout << mon.get<>(UartCommandEepromRead(600)) << std::endl;
            compareResult(
                3, mon.get<>(UartCommandEepromRead(600)).responseStruct().data);

            break;
        case 'g': {
            std::vector<uint8_t> eeprom;
            for (int i = 0; i < 1024; i++) {
                eeprom.push_back(
                    mon.get<>(UartCommandEepromRead(i)).responseStruct().data);
            }

            for (int i = 0; i < 64; i++) {
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

        } break;
        case 'A':
            testAes(mon);
            break;
        case 'C':
            mon.printCounterValues();
            break;
        case 'i': {
            std::string s(optarg);
            std::vector<uint8_t> vec(s.begin(), s.end());
            std::cout << mon.get<>(
                UartCommandI2cWrite(i2cDeviceAddress, i2cDeviceOffset, vec.size(), vec))
                      << std::endl;
        } break;
        case 'I':
            std::cout << mon.get<>(UartCommandI2cRead(
                i2cDeviceAddress, i2cDeviceOffset, atoi(optarg)))
                      << std::endl;
            break;
        case 'U': 
            {
            std::string s(optarg);
            if(s.at(0) == 's')
            {
                mon.get<>(UartCommandNrf24l01Init({0xF0, 0xF0, 0xF0, 0xF0, 0xC2}, {0xF0, 0xF0, 0xF0, 0xF0, 0xC2}, 121, true));
            }
            if(s.at(0) == 'r')
            {
                mon.get<>(UartCommandNrf24l01Init({0xF0, 0xF0, 0xF0, 0xF0, 0xC2}, {0xF0, 0xF0, 0xF0, 0xF0, 0xC2}, 121, false));
            }
            std::cout << mon.get<>(UartCommandRadioUart(s.at(0))) << std::endl;
            }
            break;
            case 'S': {
                RadioSession radioSession(mon, 0);
                radioSession.setKeepAliveInterval(20);
                radioSession.wakeupNotResponding();

                auto gatewayStats = mon.get<>(UartCommandGetStatistics());
                auto nodeStats = mon.getRadio<>(UartCommandGetStatistics());

                std::cout << "PC --> uart, gateway --> node" << std::endl;
                std::cout << mon.getBytesSent() << " --> ";
                std::cout << gatewayStats.responseStruct().getUart_rx() << ", ";
                std::cout << gatewayStats.responseStruct().getRf_tx() << " --> ";
                std::cout << nodeStats.responseStruct().getRf_rx() << std::endl;

                std::cout << mon.getBytesReceived() << " <-- ";
                std::cout << gatewayStats.responseStruct().getUart_tx() << ", ";
                std::cout << gatewayStats.responseStruct().getRf_rx() << " <-- ";
                std::cout << nodeStats.responseStruct().getRf_tx() << std::endl;
            } break;
         case 'b':
            std::cout << mon.get<>(UartCommandGpio()).getJson() << std::endl;
            std::cout << mon.get<>(UartCommandDebug()).getJson() << std::endl;
            std::cout << mon.get<>(UartCommandGetVersion()).getJson() << std::endl;
            std::cout << mon.get<>(UartCommandGetDeviceName()).getJson() << std::endl;
            std::cout << mon.get<>(UartCommandVcc()).getJson() << std::endl;
            std::cout << mon.get<>(UartCommandGetStatistics()).getJson() << std::endl;
            break;
        case 'K': {
            std::string s(optarg);
            std::vector<uint8_t> key;

            // read key ascii values
            for (uint8_t i = 0; i < s.size() & i < 16; i++) {
                key.push_back(s.at(i));
            }

            // set key
            mon.get<>(UartCommandSetKey('E', key));

            // debug
            // for (int i = 0; i < 16; i++) {
            // std::cout << mon.get<>(UartCommandEepromRead(64 + i))
            //<< std::endl;
            //}
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
                argv[i+1][0] = '\0';
            }
        }
    }

    Uart uart(deviceFile);
    EventProcess ep;
    LinuxCryptoHandler cryptoHandler;
    monitor mon(uart, ep, &cryptoHandler);

    std::thread readerThread(&EventProcess::Run, &ep);

    parseOpt(argc, argv, mon);

    readerThread.join();

    return 0;
}
