
#ifdef USE_FEATURE_MQTT
#include "mqtt/async_client.h"
#endif

#include <cmath>
#include <cmd/commands.hxx>
#include <crypto.hpp>
#include <eventprocess.hpp>
#include <filesystem>
#include <linuxCryptoHandler.hpp>
#include <monitor.hpp>
#include <radioSession.hpp>
#include <thread>
#include <uart.hpp>

using namespace std::chrono_literals;

void testDecryptOnAvr(monitor& mon)
{
    // std::cout << "encrypt on PC, decrypt on AVR" << std::endl;
    Crypto c {};
    Crypto::AesBlock key({ 's', 'e', 'c', 'r', 'e', 't' });
    c.setEncryptKey(key);

    Crypto::AesBlock plaintext({ 'T', 'e', 's', 't' });

    Crypto::AesBlock ciphertext = c.encrypt(plaintext);

    RaduinoCommandAes decryptedInAvr = mon.get<>(RaduinoCommandAes('d', c.getVector(ciphertext)));

    std::string responseString { reinterpret_cast<char*>(decryptedInAvr.responseStruct().data) };
    std::cout << "pt received   : " << responseString << std::endl;
}

void testEncryptOnAvr(monitor& mon)
{
    // std::cout << "encrypt on AVR, decrypt on PC" << std::endl;
    std::string s { "Test" };
    std::vector<uint8_t> v { s.begin(), s.end() };
    RaduinoCommandAes encryptedInAvr = mon.get<>(RaduinoCommandAes('c', v));

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
    std::cout << "           -K <key> : encrypt command with transport key" << std::endl;
    std::cout << "       -N <address> : wakeup node address" << std::endl;
    std::cout << "                 -A : test AES" << std::endl;
    std::cout << "                 -C : print counter values" << std::endl;
    std::cout << "                 -e : EEPROM command" << std::endl;
    std::cout << "                 -I : I2C read command" << std::endl;
    std::cout << "                 -s : test sha1 command" << std::endl;
    std::cout << "                 -g : dump eeprom from mega328p" << std::endl;
    std::cout << "                 -b : test json formatter" << std::endl;
    std::cout << "                 -S : rx and tx statistics for node and gateway" << std::endl;
    std::cout << "                 -t : transport encryption off" << std::endl;
    std::cout << "                 -T : transport encryption on" << std::endl;
    std::cout << "                 -V : Verbose on" << std::endl;
    std::cout << "                 -v : Verbose off" << std::endl;
    std::cout << "                 -h : print this text" << std::endl;
}

void compareResult(uint8_t expected, uint8_t actual)
{
    if (expected != actual) {
        std::cout << std::endl;
        std::cout << "ERROR: invalid result! expected: " << static_cast<int>(expected)
                  << " actual: " << static_cast<int>(actual) << std::endl;
        std::cout << std::endl;
    }
}

void parseOpt(int argc, char* argv[], monitor& mon, LinuxCryptoHandler& cryptoHandler)
{
    char option = 0;
    uint16_t i2cDeviceOffset = 0;
    uint8_t i2cDeviceAddress = 0b10100000;
    uint32_t radioAddress = 0;
    bool verbose = false;

    while ((option = getopt(argc, argv, "K:N:vVACeI:gbSshT:t")) != -1) {
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
            verbose = true;
            mon.printDebug(true);
            mon.setPrintResponseTime(true);
            break;
        case 'v':
            mon.printDebug(false);
            verbose = false;
            mon.setPrintResponseTime(false);
            break;

        case 'e':
            std::cout << mon.get<>(RaduinoCommandEepromWrite(2, 3)) << std::endl;
            std::cout << mon.get<>(RaduinoCommandEepromRead(2)) << std::endl;
            compareResult(3, mon.get<>(RaduinoCommandEepromRead(2)).responseStruct().data);

            std::cout << mon.get<>(RaduinoCommandEepromWrite(600, 3)) << std::endl;
            std::cout << mon.get<>(RaduinoCommandEepromRead(600)) << std::endl;
            compareResult(3, mon.get<>(RaduinoCommandEepromRead(600)).responseStruct().data);

            break;
        case 'g': {
            std::vector<uint8_t> eeprom;
            uint16_t printAddress = 0;
            std::cout << "addr 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  D   : ascii" << std::endl;
            for (int readAddress = 0; readAddress < 1024; readAddress++) {
                auto responseStruct = mon.get<>(RaduinoCommandEepromRead(readAddress)).responseStruct();

                if (responseStruct.getAddress() == readAddress) {
                    eeprom.push_back(responseStruct.getData());
                }
                else {
                    std::cout << "invalid response from eeprom_read command. abort." << std::endl;
                    break;
                }

                if ((eeprom.size() - printAddress) > 16) {
                    for (; printAddress < eeprom.size() / 16; printAddress++) {
                        std::cout.fill('0');
                        std::cout.width(4);
                        std::cout << std::hex << static_cast<int>(printAddress * 16) << " ";
                        for (int j = 0; j < 16; j++) {
                            std::cout.fill('0');
                            std::cout.width(2);

                            std::cout << std::hex << static_cast<int>(eeprom.at(printAddress * 16 + j));
                            std::cout << " ";
                        }
                        std::cout << " : ";
                        for (int j = 0; j < 16; j++) {
                            std::cout << std::hex << std::uppercase << eeprom.at(printAddress * 16 + j);
                        }
                        std::cout << std::endl;
                    }
                }
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
            std::cout << mon.get<>(RaduinoCommandI2cWrite(i2cDeviceAddress, i2cDeviceOffset, vec.size(), vec))
                      << std::endl;
        } break;
        case 'I':
            std::cout << mon.get<>(RaduinoCommandI2cRead(i2cDeviceAddress, i2cDeviceOffset, atoi(optarg))) << std::endl;
            break;
        case 'S': {
            RadioSession radioSession(mon, 0);
            radioSession.wakeupNotResponding();

            auto gatewayStats = mon.get<>(RaduinoCommandGetStatistics());
            auto nodeStats = mon.getRadio<>(RaduinoCommandGetStatistics());

            std::cout << "PC --> uart, gateway --> node" << std::endl;
            std::cout << mon.getBytesSent() << " --> ";
            std::cout << gatewayStats.responseStruct().getUartrx() << ", ";
            std::cout << gatewayStats.responseStruct().getRadiotx() << " --> ";
            std::cout << nodeStats.responseStruct().getRadiorx() << std::endl;

            std::cout << mon.getBytesReceived() << " <-- ";
            std::cout << gatewayStats.responseStruct().getUarttx() << ", ";
            std::cout << gatewayStats.responseStruct().getRadiorx() << " <-- ";
            std::cout << nodeStats.responseStruct().getRadiotx() << std::endl;
        } break;
        case 'b':
            std::cout << mon.get<>(RaduinoCommandGpio()).getJson() << std::endl;
            std::cout << mon.get<>(RaduinoCommandSha1("test")).getJson() << std::endl;
            std::cout << mon.get<>(RaduinoCommandHotp()).getJson() << std::endl;
            std::cout << mon.get<>(RaduinoCommandDebug()).getJson() << std::endl;
            std::cout << mon.get<>(RaduinoCommandRandom()).getJson() << std::endl;
            std::cout << mon.get<>(RaduinoCommandGetVersion()).getJson() << std::endl;
            std::cout << mon.get<>(RaduinoCommandGetDeviceName()).getJson() << std::endl;
            std::cout << mon.get<>(RaduinoCommandVcc()).getJson() << std::endl;
            std::cout << mon.get<>(RaduinoCommandGetStatistics()).getJson() << std::endl;
            std::cout << mon.get<>(RaduinoCommandPing()).getJson() << std::endl;
            break;
        case 's': {
            // printf "best\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0" | xxd
            // printf "best\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0" | sha1sum
            auto result = mon.get<>(RaduinoCommandSha1("best"));
            std::cout << result << std::endl;
            compareResult(0x31, result.responseStruct().data[0]);
            compareResult(0x02, result.responseStruct().data[1]);
            compareResult(0xcf, result.responseStruct().data[2]);
        } break;
        case 't':
            mon.setTransportEncryption(false);
            break;
        case 'T': {
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
