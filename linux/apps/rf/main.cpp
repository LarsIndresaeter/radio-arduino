
#ifdef USE_FEATURE_MQTT
#include "mqtt/async_client.h"
#endif

#include <chrono>
#include <cmath>
#include <cmd/commands.hpp>
#include <eventprocess.hpp>
#include <filesystem>
#include <linuxCryptoHandler.hpp>
#include <monitor.hpp>
#include <numeric>
#include <thread>
#include <uart.hpp>
#include <radioSession.hpp>

using namespace std::chrono_literals;

int timeMs()
{
    using namespace std::chrono;
    return (duration_cast<milliseconds>(system_clock::now().time_since_epoch())
                .count())
        % 1000;
}

void print_usage()
{
    std::cout << "raduino-node" << std::endl;
    std::cout << "       -V : Verbose on" << std::endl;
    std::cout << "       -v : Verbose off" << std::endl;
    std::cout << "       -B : Blink command" << std::endl;
    std::cout << "       -E : EEPROM command" << std::endl;
    std::cout << "       -H : HOTP command" << std::endl;
    std::cout << "       -P : pwm command" << std::endl;
    std::cout << "       -R : get random bytes command" << std::endl;
    std::cout << "       -C : print counter values" << std::endl;
    std::cout << "       -D : debug command" << std::endl;
    std::cout << "       -t : disable transport encryption" << std::endl;
    std::cout << "       -T : enable transport encryption" << std::endl;
    std::cout << "       -i : I2C write command" << std::endl;
    std::cout << "       -I : I2C read command" << std::endl;
    std::cout << "       -d : I2C device address" << std::endl;
    std::cout << "       -o : I2C device offset" << std::endl;
    std::cout << "       -M : ina219 power monitor" << std::endl;
    std::cout << "       -N : get statistics" << std::endl;
    std::cout << "       -X : ds18b20 temperature sensor" << std::endl;
    std::cout << "       -K : set AES Key" << std::endl;
    std::cout << "       -g : dump eeprom from mega328p" << std::endl;
    std::cout << "       -Z : set device name" << std::endl;
    std::cout << "       -z : get device name" << std::endl;
    std::cout << "       -W : WS2812B <string>" << std::endl;
    std::cout << "       -U : radio uart command <s> send, <r> receive" << std::endl;
    std::cout << "       -j : read vcc" << std::endl;
    std::cout << "       -s : sleep" << std::endl;
    std::cout << "       -L : print text on LCD" << std::endl;
    std::cout << "       -w : wake up sleeping rx node" << std::endl;
    std::cout << "       -x : wake up sleeping rx node if data available flag is set" << std::endl;
    std::cout << "       -q : read quadrature encoder" << std::endl;
    std::cout << "       -A : read quadrature encoder on change" << std::endl;
    std::cout << "       -h : print this text" << std::endl;
    std::cout << "       -n : wakeup node address" << std::endl;
    std::cout << "       -a : update node address" << std::endl;
    std::cout << "       -k : set keep alive interval" << std::endl;
    std::cout << "       -p : ping radio node" << std::endl;
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

void readCurrentAndVoltage(monitor& mon, int samples)
{
    auto ina219 = mon.getRadio<>(UartCommandIna219());
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
            ina219 = mon.getRadio<>(UartCommandIna219());
            intval = ina219.responseStruct().getCurrent();
            current = intval * 0.001;

            intval = ina219.responseStruct().getVoltage();
            intval = intval >> 3; // ignore 3 LSB
            voltage = intval * 0.004;

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

        double sum
            = std::accumulate(currentData.begin(), currentData.end(), 0.0);
        double mean = sum / currentData.size();

        double voltageSum
            = std::accumulate(voltageData.begin(), voltageData.end(), 0.0);
        double voltageMean = voltageSum / voltageData.size();

        std::vector<double> diff(currentData.size());
        std::transform(
            currentData.begin(),
            currentData.end(),
            diff.begin(),
            std::bind2nd(std::minus<double>(), mean));
        double sq_sum
            = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
        double stdev = std::sqrt(sq_sum / currentData.size());

        if (loopCounter > 0) {
            time_t now = std::chrono::system_clock::to_time_t(
                std::chrono::system_clock::now());
            struct tm t = *localtime(&now);
            std::cout << 1900 + t.tm_year << std::setfill('0') << "-"
                      << std::setw(2) << t.tm_mon << "-" << std::setw(2)
                      << t.tm_mday << " " << std::setw(2) << t.tm_hour << ":"
                      << std::setw(2) << t.tm_min << ":" << std::setw(2)
                      << t.tm_sec;

            std::cout << std::fixed << std::setprecision(3) << ", "
                      << currentData.size() << ", " << currentMin << ", "
                      << mean << ", " << currentMax << ", " << stdev << ", "
                      << voltageMin << ", " << voltageMean << ", " << voltageMax
                      << std::endl;
        }

        loopCounter++;
    }
}

void parseOpt(int argc, char* argv[], monitor& mon)
{
    char option = 0;
    uint16_t i2cDeviceOffset = 0;
    uint8_t i2cDeviceAddress = 0b10100000;
    uint8_t radioAddress = 0;
    uint8_t keepAliveInterval = 0;
    bool verbose = false;

    while ((option
            = getopt(argc, argv, "P:DBHECs:Rd:VvhtTgGi:I:o:MNXK:Z:zW:wxqAL:JU:jn:a:k:p"))
           != -1) {
        switch (option) {
        case 'd':
            i2cDeviceAddress = atoi(optarg);
            break;
        case 'o':
            i2cDeviceOffset = atoi(optarg);
            break;
        case 's':
            {
            uint32_t delay = atoi(optarg);
            std::cout << mon.getRadio<>(UartCommandSleep(delay), static_cast<std::chrono::milliseconds>(delay + 2000)) << std::endl;
            }
            break;
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
        case 'p':
        {
            RadioSession radioSession(mon, radioAddress);
            radioSession.setKeepAliveInterval(keepAliveInterval);
            radioSession.wakeupNotResponding();
            std::cout << mon.getRadio<>(UartCommandPing()) << std::endl;
        } break;
        case 'w':
            std::cout << mon.get<>(UartCommandWakeup(false), static_cast<std::chrono::milliseconds>(12000)) << std::endl;
            break;
        case 'x':
            std::cout << mon.get<>(UartCommandWakeup(true), static_cast<std::chrono::milliseconds>(12000)) << std::endl;
            break;
        case 'q':
            std::cout << mon.getRadio<>(UartCommandQuadratureEncoder()) << std::endl;
            break;
        case 'A': {
            while (1) {
                mon.get<>(UartCommandWakeup(true), static_cast<std::chrono::milliseconds>(12000));
                if (mon.lastCommandReturnedValidResponse()) {
                    std::cout << mon.getRadio<>(UartCommandQuadratureEncoder()) << std::endl;
                }
            }
        } break;
        case 'v':
            mon.printDebug(false);
            verbose = false;
            mon.setPrintResponseTime(false);
            break;
        case 'B':
            std::cout << mon.getRadio<>(UartCommandBlink()) << std::endl;
            break;
        case 'O':
            {
                std::cout << "blink : " << mon.getRadio<>(UartCommandBlink()) << std::endl;
                std::cout << "gpio  : " << mon.getRadio<>(UartCommandGpio()) << std::endl;
                std::cout << "blink : " << mon.getRadio<>(UartCommandBlink()) << std::endl;
                std::cout << "gpio  : " << mon.getRadio<>(UartCommandGpio()) << std::endl;
                std::cout << "device: " << mon.getRadio<>(UartCommandGetDeviceName()) << std::endl;
            }
            break;
        case 'J':
                {
                std::vector<uint8_t> ackPayload;
                for(int i = 0; i<1000;i++){
                    auto rec = mon.getRadio<>(UartCommandNrf24l01Read(ackPayload.size(), ackPayload));
                    ackPayload.clear();

                    std::this_thread::sleep_for(100ms);

                    if(rec.responseStruct().length > 0)
                    {
                        std::cout << rec << std::endl;

                        for(int j = 0; j<rec.responseStruct().length; j++)
                        {
                            ackPayload.push_back(rec.responseStruct().data[j]);
                        }
                    }
                }
            }

            break;
        case 'E':
            std::cout << mon.getRadio<>(UartCommandEepromWrite(2, 3)) << std::endl;
            std::cout << mon.getRadio<>(UartCommandEepromRead(2)) << std::endl;
            compareResult(
                3, mon.getRadio<>(UartCommandEepromRead(2)).responseStruct().data);

            std::cout << mon.getRadio<>(UartCommandEepromWrite(600, 3)) << std::endl;
            std::cout << mon.getRadio<>(UartCommandEepromRead(600)) << std::endl;
            compareResult(
                3, mon.getRadio<>(UartCommandEepromRead(600)).responseStruct().data);

            break;
        case 'g': {
            std::vector<uint8_t> eeprom;
            for (int i = 0; i < 1024; i++) {
                eeprom.push_back(
                    mon.getRadio<>(UartCommandEepromRead(i)).responseStruct().data);
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
        case 'L': {
            COMMANDS::SSD1306::command_t command;

            std::string s(optarg);
            std::vector<uint8_t> lcd(sizeof(command.data), ' ');

            for (uint8_t i = 0; i < s.size() && i < static_cast<uint8_t>(sizeof(command.data)); i++) {
                lcd.at(i) = s.at(i);
            }

            mon.getRadio<>(UartCommandSsd1306(2, lcd)); // second line
        } break;
        case 'H':
            std::cout << mon.getRadio<>(UartCommandHotp()) << std::endl;
            break;
        case 'P': {
            uint8_t value = atoi(optarg);
            std::cout << mon.getRadio<>(UartCommandPwm('b', 2, value)) << std::endl;
        } break;
        case 'G':
            std::cout << mon.getRadio<>(UartCommandGpio()) << std::endl;
            break;
        case 'R':
            std::cout << mon.getRadio<>(UartCommandRandom()) << std::endl;
            break;
        case 'C':
            mon.printCounterValues();
            break;
        case 'D':
            std::cout << mon.getRadio<>(UartCommandDebug()) << std::endl;
            break;
        case 'j':
            {
            RadioSession radioSession(mon, radioAddress);
            radioSession.wakeupNotResponding();
            std::cout << mon.getRadio<>(UartCommandVcc()) << std::endl;
            }
            break;
        case 'i': {
            std::string s(optarg);
            std::vector<uint8_t> vec(s.begin(), s.end());
            std::cout << mon.getRadio<>(
                UartCommandI2cWrite(i2cDeviceAddress, i2cDeviceOffset, vec.size(), vec))
                      << std::endl;
        } break;
        case 'I':
            std::cout << mon.getRadio<>(UartCommandI2cRead(
                i2cDeviceAddress, i2cDeviceOffset, atoi(optarg)))
                      << std::endl;
            break;
        case 'M':
            std::cout << mon.getRadio<>(UartCommandIna219()) << std::endl;
            break;
        case 'N':
            std::cout << mon.get<>(UartCommandGetStatistics()) << std::endl;
            break;
        case 'X':
            std::cout << mon.getRadio<>(UartCommandDs18b20()) << std::endl;
            break;
        case 'U': 
            {
            std::string s(optarg);
            if(s.at(0) == 's')
            {
                mon.getRadio<>(UartCommandNrf24l01Init({0xF0, 0xF0, 0xF0, 0xF0, 0xC2}, {0xF0, 0xF0, 0xF0, 0xF0, 0xC2}, 121, true));
            }
            if(s.at(0) == 'r')
            {
                mon.getRadio<>(UartCommandNrf24l01Init({0xF0, 0xF0, 0xF0, 0xF0, 0xC2}, {0xF0, 0xF0, 0xF0, 0xF0, 0xC2}, 121, false));
            }
            std::cout << mon.getRadio<>(UartCommandRadioUart(s.at(0))) << std::endl;
            }
            break;
        case 'K': {
            std::string s(optarg);
            std::vector<uint8_t> key;

            // read key ascii values
            for (uint8_t i = 0; i < s.size() & i < 16; i++) {
                key.push_back(s.at(i));
            }

            // set key
            mon.getRadio<>(UartCommandSetKey('E', key));
        } break;
        case 'Z': {
            std::string s(optarg);
            std::vector<uint8_t> name;

            // read name ascii values
            for (uint8_t i = 0; i < s.size() & i < 16; i++) {
                name.push_back(s.at(i));
            }

            // set name
            mon.getRadio<>(UartCommandSetDeviceName(name));
        } break;
        case 'z': {
            RadioSession radioSession(mon, radioAddress);
            radioSession.setKeepAliveInterval(keepAliveInterval);
            radioSession.wakeupNotResponding();
            std::cout << mon.getRadio<>(UartCommandGetDeviceName()) << std::endl;
        } break;
        case 'h':
            print_usage();
            break;
        case 'n':
            radioAddress = atoi(optarg);
            {
                RadioSession radioSession(mon, radioAddress);
                radioSession.setKeepAliveInterval(keepAliveInterval);
                if(verbose)
                {
                    radioSession.setVerbose(true);
                }
                radioSession.wakeupNotResponding();
            }
            break;
        case 'a':
            std::cout << mon.getRadio<>(UartCommandSetNodeAddress(atoi(optarg))) << std::endl;
            break;
        case 'k':
            keepAliveInterval = atoi(optarg);
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
