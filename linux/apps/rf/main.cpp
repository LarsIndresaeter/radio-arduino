
#ifdef USE_FEATURE_MQTT
#include "mqtt/async_client.h"
#endif

#include <chrono>
#include <cmath>
#include <commands.hpp>
#include <eventprocess.hpp>
#include <filesystem>
#include <linuxCryptoHandler.hpp>
#include <monitor.hpp>
#include <numeric>
#include <thread>
#include <uart.hpp>

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
    std::cout << "rf-tool" << std::endl;
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
    std::cout << "       -N : ina219 power monitor, statistic per second for "
                 "<N> seconds"
              << std::endl;
    std::cout << "       -X : ds18b20 temperature sensor" << std::endl;
    std::cout << "       -K : set AES Key" << std::endl;
    std::cout << "       -g : dump eeprom from mega328p" << std::endl;
    std::cout << "       -Z : set device name" << std::endl;
    std::cout << "       -z : get device name" << std::endl;
    std::cout << "       -W : WS2812B <string>" << std::endl;
    std::cout << "       -F : Read servo PWM signal and display on ws2812b ring"
              << std::endl;
    std::cout << "       -U : radio uart command <s> send, <r> receive" << std::endl;
    std::cout << "       -j : read vcc" << std::endl;
    std::cout << "       -s : sleep" << std::endl;
    std::cout << "       -L : print text on LCD" << std::endl;
    std::cout << "       -w : wake up sleeping rx slave" << std::endl;
    std::cout << "       -h : print this text" << std::endl;
    std::cout << "       -m : set master address" << std::endl;
    std::cout << "       -a : set slave address" << std::endl;
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
            intval = (int16_t)(
                ((uint16_t)ina219.responseStruct().current[0]) << 8
                | ina219.responseStruct().current[1]);
            current = intval * 0.001;

            intval = (int16_t)(
                ((uint16_t)ina219.responseStruct().voltage[0]) << 8
                | ina219.responseStruct().voltage[1]);
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

    while ((option
            = getopt(argc, argv, "P:DBHECs:Rd:VvhtTgGi:I:o:MN:XK:Z:zW:wL:FJU:jm:a:"))
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
            mon.printDebug(true);
            mon.setPrintResponseTime(true);
            break;
        case 't':
            mon.setTransportEncryption(false);
            break;
        case 'T':
            mon.setTransportEncryption(true);
            break;
        case 'w':
            std::cout << mon.get<>(UartCommandWakeup(), static_cast<std::chrono::milliseconds>(12000)) << std::endl;
            break;
        case 'W': {
            UartCommandWs2812b ws2812b;
            std::string s(optarg);

            if (s.at(0) == 'a') {
                using namespace std::chrono;
                int random_variable
                    = duration_cast<milliseconds>(
                          system_clock::now().time_since_epoch())
                            .count()
                        % 90
                    + 45;
                for (int k = 0; k < random_variable; k++) {
                    for (int i = 0; i < COMMANDS::WS2812B::LEDS; i++) {
                        if (i % 9 < 3) {
                            ws2812b.setLed(i, 1, 0, 0);
                        }
                        else if (i % 9 > 5) {
                            ws2812b.setLed(i, 0, 1, 0);
                        }
                        else {
                            ws2812b.setLed(i, 0, 0, 1);
                        }
                    }
                    ws2812b.setLed(k % (COMMANDS::WS2812B::LEDS), 8, 8, 8);
                    mon.getRadio<>(ws2812b);
                    std::this_thread::sleep_for((k + 5) * 1ms);
                }
            }
            else {
                for (uint8_t i = 0; i < s.size() & i < COMMANDS::WS2812B::LEDS;
                     i++) {
                    if (s.at(i) == 'w') {
                        ws2812b.setLed(i, 32, 32, 32);
                    }
                    else if (s.at(i) == 'r') {
                        ws2812b.setLed(i, 32, 0, 0);
                    }
                    else if (s.at(i) == 'g') {
                        ws2812b.setLed(i, 0, 32, 0);
                    }
                    else if (s.at(i) == 'b') {
                        ws2812b.setLed(i, 0, 0, 32);
                    }
                    else if (s.at(i) == 'd') {
                        ws2812b.setLed(i, 0, 0, 0);
                    }
                }
            }

            mon.getRadio<>(ws2812b);
        } break;
        case 'v':
            mon.printDebug(false);
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
                std::cout << "device: " << mon.getRadio<>(UartCommandGetDeviceInfo()) << std::endl;
            }
            break;
        case 'J':
                {
                std::vector<uint8_t> ackPayload;
                for(int i = 0; i<1000;i++){
                    auto rec = mon.getRadio<>(UartCommandNrf24l01Read(ackPayload));
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
            std::string s(optarg);
            std::vector<uint8_t> lcd(COMMANDS::SSD1306::STRING_LENGTH, ' ');

            for (uint8_t i = 0; i < s.size() && i < COMMANDS::SSD1306::STRING_LENGTH; i++) {
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
        case 'F': {
            while (true) {
                auto r = mon.getRadio<>(UartCommandTimer());

                uint16_t pulse_width = r.responseStruct().pulse_width_high << 8;
                pulse_width |= r.responseStruct().pulse_width_low;

                std::cout << "pulse_width=" << pulse_width << std::endl;

                if ((pulse_width > 1000) && (pulse_width < 2000)) {
                    UartCommandWs2812b ws2812b;
                    int v = (pulse_width - 1000) / 22;
                    for (int i = 0; i < v; i++) {
                        ws2812b.setLed(i, 1, 1, 1);
                    }

                    mon.getRadio<>(ws2812b);
                }
            }
        }
        case 'D':
            std::cout << mon.getRadio<>(UartCommandDebug()) << std::endl;
            break;
        case 'j':
            std::cout << mon.getRadio<>(UartCommandVcc()) << std::endl;
            break;
        case 'i': {
            std::string s(optarg);
            std::vector<uint8_t> vec(s.begin(), s.end());
            std::cout << mon.getRadio<>(
                UartCommandI2cWrite(i2cDeviceAddress, i2cDeviceOffset, vec))
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
            readCurrentAndVoltage(mon, atoi(optarg));
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
            mon.getRadio<>(UartCommandSetKey(COMMANDS::SET_KEY::EK, key));
        } break;
        case 'Z': {
            std::string s(optarg);
            std::vector<uint8_t> name;

            // read name ascii values
            for (uint8_t i = 0; i < s.size() & i < 16; i++) {
                name.push_back(s.at(i));
            }

            // set name
            mon.getRadio<>(UartCommandSetDeviceInfo(name));
        } break;
        case 'z':
            std::cout << mon.getRadio<>(UartCommandGetDeviceInfo()) << std::endl;
            break;
        case 'h':
            print_usage();
            break;
        case 'm':
            std::cout << mon.get<>(UartCommandSetSlaveAddress(atoi(optarg))) << std::endl;
            break;
        case 'a':
            std::cout << mon.getRadio<>(UartCommandSetSlaveAddress(atoi(optarg))) << std::endl;
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
