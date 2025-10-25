
#ifdef USE_FEATURE_MQTT
#include "mqtt/async_client.h"
#endif

#include <chrono>
#include <cmath>
#include <cmd/commands.hxx>
#include <eventprocess.hpp>
#include <filesystem>
#include <linuxCryptoHandler.hpp>
#include <monitor.hpp>
#include <numeric>
#include <radioSession.hpp>
#include <thread>
#include <uart.hpp>

using namespace std::chrono_literals;

int timeMs()
{
    using namespace std::chrono;
    return (duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count()) % 1000;
}

void print_usage()
{
    std::cout << "raduino-node" << std::endl;
    std::cout << "       -V : Verbose on" << std::endl;
    std::cout << "       -v : Verbose off" << std::endl;
    std::cout << "       -B : Blink command" << std::endl;
    std::cout << "       -e : EEPROM command" << std::endl;
    std::cout << "       -H : HOTP command" << std::endl;
    std::cout << "       -P : pwm command" << std::endl;
    std::cout << "       -R : get random bytes command" << std::endl;
    std::cout << "       -C : print counter values" << std::endl;
    std::cout << "       -D : debug command" << std::endl;
    std::cout << "       -t : disable transport encryption" << std::endl;
    std::cout << "       -T : enable transport encryption" << std::endl;
    std::cout << "       -x : get statistics" << std::endl;
    std::cout << "       -E : set AES Key" << std::endl;
    std::cout << "       -g : reboot node as gateway" << std::endl;
    std::cout << "       -Z : set device name" << std::endl;
    std::cout << "       -z : get device name" << std::endl;
    std::cout << "       -U : radio uart command <s> send, <r> receive" << std::endl;
    std::cout << "       -j : read vcc" << std::endl;
    std::cout << "       -s : sleep" << std::endl;
    std::cout << "       -w : wake up sleeping rx node" << std::endl;
    std::cout << "       -N : wake up sleeping rx node if data available flag is set" << std::endl;
    std::cout << "       -q : read quadrature encoder" << std::endl;
    std::cout << "       -A : read quadrature encoder on change" << std::endl;
    std::cout << "       -n : wakeup node address" << std::endl;
    std::cout << "       -a : update node address" << std::endl;
    std::cout << "       -k : set keep alive interval" << std::endl;
    std::cout << "       -p : ping radio node" << std::endl;
    std::cout << "       -K : set transport key on device (command must be encrypted)" << std::endl;
    std::cout << "       -b : use transport key" << std::endl;
    std::cout << "       -r : set transport encryption required (command must be encrypted)" << std::endl;
    std::cout << "       -u : unencrypted session (command must be encrypted)" << std::endl;
    std::cout << "       -h : print this text" << std::endl;
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

void readCurrentAndVoltage(monitor& mon, int samples)
{
    auto ina219 = mon.getRadio<>(RaduinoCommandIna219());
    std::vector<float> currentData, voltageData;
    int16_t intval = 0;
    float currentMin, currentMax, current, stdev;
    float voltageMin, voltageMax, voltage;
    int time, timePrev;
    int loopCounter = 0;

    for (int i = 0; i <= samples; i++) {
        time = 1;
        timePrev = 0;
        currentMin = 1000.0;
        currentMax = 0.0;
        voltageMin = 50.0;
        voltageMax = 0.0;
        currentData.clear();
        voltageData.clear();
        while (time > timePrev) {
            ina219 = mon.getRadio<>(RaduinoCommandIna219());
            intval = ina219.responseStruct().getCurrent();
            current = ((int16_t)intval) * 0.001;

            intval = ina219.responseStruct().getVoltage();
            intval = intval >> 3; // ignore 3 LSB
            voltage = ((int16_t)intval) * 0.004;

            if (current < currentMin) {
                currentMin = current;
            }
            if (current > currentMax) {
                currentMax = current;
            }

            if (voltage < voltageMin) {
                voltageMin = voltage;
            }
            if (voltage > voltageMax) {
                voltageMax = voltage;
            }

            currentData.push_back(current);
            voltageData.push_back(voltage);
            timePrev = time;
            time = timeMs();
        }

        double sum = std::accumulate(currentData.begin(), currentData.end(), 0.0);
        double mean = sum / currentData.size();

        double voltageSum = std::accumulate(voltageData.begin(), voltageData.end(), 0.0);
        double voltageMean = voltageSum / voltageData.size();

        std::vector<double> diff(currentData.size());
        std::transform(currentData.begin(), currentData.end(), diff.begin(), std::bind2nd(std::minus<double>(), mean));
        double sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
        double stdev = std::sqrt(sq_sum / currentData.size());

        if (loopCounter > 0) {
            time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            struct tm t = *localtime(&now);
            std::cout << 1900 + t.tm_year << std::setfill('0') << "-" << std::setw(2) << t.tm_mon << "-" << std::setw(2)
                      << t.tm_mday << " " << std::setw(2) << t.tm_hour << ":" << std::setw(2) << t.tm_min << ":"
                      << std::setw(2) << t.tm_sec;

            std::cout << std::fixed << std::setprecision(3) << ", " << currentData.size() << ", " << currentMin << ", "
                      << mean << ", " << currentMax << ", " << stdev << ", " << voltageMin << ", " << voltageMean
                      << ", " << voltageMax << std::endl;
        }

        loopCounter++;
    }
}

void parseOpt(int argc, char* argv[], monitor& mon, LinuxCryptoHandler& cryptoHandler)
{
    char option = 0;
    uint8_t radioAddress = 0;
    uint8_t keepAliveInterval = 0;
    bool verbose = false;

    while ((option = getopt(argc, argv, "P:DBHeCs:RVvhtTgGNE:Z:zwxqAU:jn:a:k:pr:b:K:u")) != -1) {
        switch (option) {
        case 's': {
            uint32_t delay = atoi(optarg);
            std::cout << mon.getRadio<>(
                RaduinoCommandSleep(delay), static_cast<std::chrono::milliseconds>(delay + 2000))
                      << std::endl;
        } break;
        case 'V':
            verbose = true;
            mon.printDebug(true);
            mon.setPrintResponseTime(true);
            break;
        case 't':
            mon.setTransportEncryption(false);
            break;
        case 'T':
            mon.setTransportEncryption(true);
            break;
        case 'p': {
            RadioSession radioSession(mon, radioAddress);
            radioSession.setKeepAliveInterval(keepAliveInterval);
            radioSession.wakeupNotResponding();
            std::cout << mon.getRadio<>(RaduinoCommandPing()) << std::endl;
        } break;
        case 'w':
            std::cout << mon.get<>(RaduinoCommandWakeup(false), static_cast<std::chrono::milliseconds>(12000))
                      << std::endl;
            break;
        case 'N':
            std::cout << mon.get<>(RaduinoCommandWakeup(true), static_cast<std::chrono::milliseconds>(12000))
                      << std::endl;
            break;
        case 'q':
            std::cout << mon.getRadio<>(RaduinoCommandQuadratureEncoder()) << std::endl;
            break;
        case 'A': {
            while (1) {
                mon.get<>(RaduinoCommandWakeup(true), static_cast<std::chrono::milliseconds>(12000));
                if (mon.lastCommandReturnedValidResponse()) {
                    std::cout << mon.getRadio<>(RaduinoCommandQuadratureEncoder()) << std::endl;
                }
            }
        } break;
        case 'v':
            mon.printDebug(false);
            verbose = false;
            mon.setPrintResponseTime(false);
            break;
        case 'B':
            std::cout << mon.getRadio<>(RaduinoCommandBlink()) << std::endl;
            break;
        case 'O': {
            std::cout << "blink : " << mon.getRadio<>(RaduinoCommandBlink()) << std::endl;
            std::cout << "gpio  : " << mon.getRadio<>(RaduinoCommandGpio()) << std::endl;
            std::cout << "blink : " << mon.getRadio<>(RaduinoCommandBlink()) << std::endl;
            std::cout << "gpio  : " << mon.getRadio<>(RaduinoCommandGpio()) << std::endl;
            std::cout << "device: " << mon.getRadio<>(RaduinoCommandGetDeviceName()) << std::endl;
        } break;
        case 'e':
            std::cout << mon.getRadio<>(RaduinoCommandEepromWrite(2, 3)) << std::endl;
            std::cout << mon.getRadio<>(RaduinoCommandEepromRead(2)) << std::endl;
            compareResult(3, mon.getRadio<>(RaduinoCommandEepromRead(2)).responseStruct().data);

            std::cout << mon.getRadio<>(RaduinoCommandEepromWrite(600, 3)) << std::endl;
            std::cout << mon.getRadio<>(RaduinoCommandEepromRead(600)) << std::endl;
            compareResult(3, mon.getRadio<>(RaduinoCommandEepromRead(600)).responseStruct().data);

            break;
        case 'g': {
            std::cout << mon.getRadio<>(RaduinoCommandSetRadioRole('g')) << std::endl;
            std::cout << mon.getRadio<>(RaduinoCommandSoftReset()) << std::endl;
        } break;
        case 'H':
            std::cout << mon.getRadio<>(RaduinoCommandHotp()) << std::endl;
            break;
        case 'P': {
            uint8_t value = atoi(optarg);
            std::cout << mon.getRadio<>(RaduinoCommandPwm('b', 2, value)) << std::endl;
        } break;
        case 'G':
            std::cout << mon.getRadio<>(RaduinoCommandGpio()) << std::endl;
            break;
        case 'R':
            std::cout << mon.getRadio<>(RaduinoCommandRandom()) << std::endl;
            break;
        case 'C':
            mon.printCounterValues();
            break;
        case 'D':
            std::cout << mon.getRadio<>(RaduinoCommandDebug()) << std::endl;
            break;
        case 'j': {
            RadioSession radioSession(mon, radioAddress);
            radioSession.wakeupNotResponding();
            std::cout << mon.getRadio<>(RaduinoCommandVcc()) << std::endl;
        } break;
        case 'x':
            std::cout << mon.getRadio<>(RaduinoCommandGetStatistics()) << std::endl;
            break;
        case 'U': {
            std::string s(optarg);
            if (s.at(0) == 's') {
                std::vector<uint8_t> address = { 0xF0, 0xF0, 0xF0, 0xF0, 0xC2 };
                mon.getRadio<>(RaduinoCommandNrf24l01Init(address, address, 121, true));
            }
            if (s.at(0) == 'r') {
                std::vector<uint8_t> address = { 0xF0, 0xF0, 0xF0, 0xF0, 0xC2 };
                mon.getRadio<>(RaduinoCommandNrf24l01Init(address, address, 121, false));
            }
            std::cout << mon.getRadio<>(RaduinoCommandRadioUart(s.at(0))) << std::endl;
        } break;
        case 'E': {
            std::string s(optarg);
            mon.getRadio<>(RaduinoCommandSetKey('E', s));
        } break;
        case 'Z': {
            std::string s(optarg);
            mon.getRadio<>(RaduinoCommandSetDeviceName(s));
        } break;
        case 'z': {
            RadioSession radioSession(mon, radioAddress);
            radioSession.setKeepAliveInterval(keepAliveInterval);
            radioSession.wakeupNotResponding();
            std::cout << mon.getRadio<>(RaduinoCommandGetDeviceName()) << std::endl;
        } break;
        case 'h':
            print_usage();
            break;
        case 'n':
            radioAddress = atoi(optarg);
            {
                RadioSession radioSession(mon, radioAddress);
                radioSession.setKeepAliveInterval(keepAliveInterval);
                if (verbose) {
                    radioSession.setVerbose(true);
                }
                radioSession.wakeupNotResponding();
            }
            break;
        case 'a':
            std::cout << mon.getRadio<>(RaduinoCommandSetNodeAddress(atoi(optarg))) << std::endl;
            break;
        case 'k':
            keepAliveInterval = atoi(optarg);
            break;
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
            std::cout << mon.getRadio<>(RaduinoCommandRequireTransportEncryption(flag)) << std::endl;
        } break;
        case 'u':
            std::cout << mon.getRadio<>(RaduinoCommandUnlockSession()) << std::endl;
            break;
        case 'K': {
            std::string s(optarg);
            mon.getRadio<>(RaduinoCommandSetKey('T', s));
        } break;
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
