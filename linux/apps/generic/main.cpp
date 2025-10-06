
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

using namespace std::chrono_literals;

#ifdef USE_FEATURE_MQTT
void sendMqtt(std::string payload)
{
    const std::string DFLT_ADDRESS { "tcp://localhost:1883" };
    const std::string TOPIC { "sensor/power" };
    const int QOS = 1;
    const std::string PERSIST_DIR { "data-persist" };
    const auto PERIOD = std::chrono::seconds(5);
    const int MAX_BUFFERED_MSGS = 120; // 120 * 5sec => 10min off-line buffering

    std::string address = DFLT_ADDRESS;
    mqtt::async_client cli(address, "", MAX_BUFFERED_MSGS, 0);

    mqtt::connect_options connOpts;
    connOpts.set_keep_alive_interval(MAX_BUFFERED_MSGS * PERIOD);
    connOpts.set_clean_session(true);
    connOpts.set_automatic_reconnect(true);

    mqtt::topic top(cli, TOPIC, QOS, true);

    try {
        cli.connect(connOpts)->wait();

        top.publish(std::move(payload));

        cli.disconnect()->wait();
    }
    catch (const mqtt::exception& exc) {
        std::cerr << exc.what() << std::endl;
    }
}
#endif

int timeMs()
{
    using namespace std::chrono;
    return (duration_cast<milliseconds>(system_clock::now().time_since_epoch())
                .count())
        % 1000;
}

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
    std::cout << "       -i : I2C write command" << std::endl;
    std::cout << "       -I : I2C read command" << std::endl;
    std::cout << "       -d : I2C device address" << std::endl;
    std::cout << "       -o : I2C device offset" << std::endl;
    std::cout << "       -M : ina219 power monitor" << std::endl;
    std::cout << "       -N : ina219 power monitor, statistic per second for "
                 "<N> seconds"
              << std::endl;
    std::cout << "       -X : ds18b20 temperature sensor" << std::endl;
    std::cout << "       -K : set encryption key" << std::endl;
    std::cout << "       -A : set transport key on device" << std::endl;
    std::cout << "       -E : use transport key" << std::endl;
    std::cout << "       -e : set transport encryption required" << std::endl;
    std::cout << "       -Z : set device name" << std::endl;
    std::cout << "       -z : get device name" << std::endl;
    std::cout << "       -a : get device version" << std::endl;
    std::cout << "       -x : get statistics" << std::endl;
    std::cout << "       -W : WS2812B <string>" << std::endl;
    std::cout << "       -F : Read servo PWM signal and display on ws2812b ring"
              << std::endl;
    std::cout << "       -U : radio uart command <s> send, <r> receive" << std::endl;
    std::cout << "       -j : read vcc" << std::endl;
    std::cout << "       -s : sleep" << std::endl;
    std::cout << "       -L : print text on LCD" << std::endl;
    std::cout << "       -p : ping command" << std::endl;
    std::cout << "       -h : print this text" << std::endl;
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
    auto ina219 = mon.get<>(UartCommandIna219());
    std::vector<float> currentData, voltageData;
    int16_t intval = 0;
    float currentMin, currentMax, current, stdev;
    float voltageMin, voltageMax, voltage;
    int time, timePrev;
    int loopCounter = 0;

#ifndef USE_FEATURE_MQTT
    std::cout
        << "timestamp          , sam, currentMin  , currentAverage , "
           "currentMax  , currentStd, minVoltage, averageVoltage, maxVoltage"
        << std::endl;
#endif
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
            ina219 = mon.get<>(UartCommandIna219());
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
#ifdef USE_FEATURE_MQTT
            sendMqtt(
                "{\"voltage\":" + std::to_string(voltageMean)
                + ", \"current\":" + std::to_string(mean) + "}");
#else
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
#endif
        }

        loopCounter++;
    }
}

void parseOpt(int argc, char* argv[], monitor& mon, LinuxCryptoHandler& cryptoHandler)
{
    char option = 0;
    uint16_t i2cDeviceOffset = 0;
    uint8_t i2cDeviceAddress = 0b10100000;

    while ((option
           = getopt(argc, argv, "P:DBSHCs:Rd:VvhtTgGi:I:o:MN:XK:A:E:e:Z:zW:L:FJU:jpax"))
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
            std::cout << mon.get<>(UartCommandSleep(delay), static_cast<std::chrono::milliseconds>(delay + 2000)) << std::endl;
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
        case 'W': {
            std::vector<uint8_t> red(45);
            std::vector<uint8_t> green(45);
            std::vector<uint8_t> blue(45);
            COMMANDS::WS2812B::command_t command;
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
                    for (int i = 0; i < sizeof(command.red); i++) {
                        if (i % 9 < 3) {
                            red.at(i) = 1;
                        }
                        else if (i % 9 > 5) {
                            green.at(i) = 1;
                        }
                        else {
                            blue.at(i) = 1;
                        }
                    }

                    red.at(k) = 8;
                    green.at(k) = 8;
                    blue.at(k) = 8;
                    UartCommandWs2812b ws2812b(red, green, blue);
                    mon.get<>(ws2812b);
                    std::this_thread::sleep_for((k + 5) * 1ms);
                }
            }

        } break;
        case 'v':
            mon.printDebug(false);
            mon.setPrintResponseTime(false);
            break;
        case 'B':
            std::cout << mon.get<>(UartCommandBlink()) << std::endl;
            break;
        case 'J':
                {
                //mon.get<>(UartCommandNrf24l01Init({0xF0, 0xF0, 0xF0, 0xF0, 0xC2}, {0xF0, 0xF0, 0xF0, 0xF0, 0xC2}, 121, false));
           
                std::vector<uint8_t> ackPayload;
                for(int i = 0; i<1000;i++){
                    auto rec = mon.get<>(UartCommandNrf24l01Read(ackPayload.size(), ackPayload));
                    ackPayload.clear();

                    std::this_thread::sleep_for(100ms);

                    if(rec.responseStruct().length > 0)
                    {
                    //std::cout << "==== STATUS      " << mon.get<>(UartCommandSpiRead(0x07, 1)) << std::endl;
                        std::cout << rec << std::endl;

                        for(int j = 0; j<rec.responseStruct().length; j++)
                        {
                            ackPayload.push_back(rec.responseStruct().data[j]);
                        }
                    }
                }
            }

            break;
        case 'S': {
            // printf "best\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0" | xxd
            // printf "best\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0" | sha1sum
            std::string s { "best" };
            std::vector<uint8_t> v { s.begin(), s.end() };
            auto result = mon.get<>(UartCommandSha1(v));
            std::cout << result << std::endl;
            compareResult(
                0x31, result.responseStruct().data[0]);
            compareResult(
                0x02, result.responseStruct().data[1]);
            compareResult(
                0xcf, result.responseStruct().data[2]);
        } break;
        case 'L': {
            COMMANDS::SSD1306::command_t command;

            std::string s(optarg);
            std::vector<uint8_t> lcd(sizeof(command.data), ' ');

            for (uint8_t i = 0; i < s.size() && i < static_cast<uint8_t>(sizeof(command.data)); i++) {
                lcd.at(i) = s.at(i);
            }

            mon.get<>(UartCommandSsd1306(2, lcd)); // second line
        } break;
        case 'H':
            std::cout << mon.get<>(UartCommandHotp()) << std::endl;
            break;
        case 'P': {
            uint8_t value = atoi(optarg);
            std::cout << mon.get<>(UartCommandPwm('b', 2, value)) << std::endl;
        } break;
        case 'G':
            std::cout << mon.get<>(UartCommandGpio()) << std::endl;
            break;
        case 'R':
            std::cout << mon.get<>(UartCommandRandom()) << std::endl;
            break;
        case 'C':
            mon.printCounterValues();
            break;
        case 'F': {
            while (true) {
                auto r = mon.get<>(UartCommandTimer());

                uint16_t pulse_width = r.responseStruct().getPulsewidth();

                std::cout << "pulse_width=" << pulse_width << std::endl;

                if ((pulse_width > 1000) && (pulse_width < 2000)) {
                    std::vector<uint8_t> red(45);
                    std::vector<uint8_t> green(45);
                    std::vector<uint8_t> blue(45);

                    int v = (pulse_width - 1000) / 22;
                    for (int i = 0; i < v; i++) {
                        red.at(i) = 1;
                        green.at(i) = 1;
                        blue.at(i) = 1;
                    }

                    UartCommandWs2812b ws2812b(red, green, blue);
                    mon.get<>(ws2812b);
                }
            }
        }
        case 'D':
            std::cout << mon.get<>(UartCommandDebug()) << std::endl;
            break;
        case 'p':
            std::cout << mon.get<>(UartCommandPing()) << std::endl;
            break;
        case 'j':
            std::cout << mon.get<>(UartCommandVcc()) << std::endl;
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
        case 'M':
            std::cout << mon.get<>(UartCommandIna219()) << std::endl;
            break;
        case 'N':
            readCurrentAndVoltage(mon, atoi(optarg));
            break;
        case 'X':
            std::cout << mon.get<>(UartCommandDs18b20()) << std::endl;
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
        case 'A': {
            std::string s(optarg);
            std::vector<uint8_t> key;

            // read key ascii values
            for (uint8_t i = 0; i < s.size() & i < 16; i++) {
                key.push_back(s.at(i));
            }

            // set key
            mon.get<>(UartCommandSetKey('T', key));
        } break;
        case 'E': {
            std::string s(optarg);
            std::vector<uint8_t> key;

            // read key ascii values
            for (uint8_t i = 0; i < s.size() & i < 16; i++) {
                key.push_back(s.at(i));
            }

            // set key
            cryptoHandler.setTransportKey((uint8_t*)&key[0]);
            cryptoHandler.setMacKey((uint8_t*)&key[0]);
        } break;
        case 'e': {
            uint8_t flag = atoi(optarg);
            std::cout << mon.get<>(UartCommandRequireTransportEncryption(flag), 0) << std::endl;
        } break;
        case 'Z': {
            std::string s(optarg);
            std::vector<uint8_t> name;

            // read name ascii values
            for (uint8_t i = 0; i < s.size() & i < 16; i++) {
                name.push_back(s.at(i));
            }

            // set name
            mon.get<>(UartCommandSetDeviceName(name));

            // debug
            // for (int i = 0; i < 16; i++) {
            // std::cout << mon.get<>(UartCommandEepromRead(64 + i))
            //<< std::endl;
            //}
        } break;
        case 'z':
            std::cout << mon.get<>(UartCommandGetDeviceName()) << std::endl;
            break;
       case 'a':
            std::cout << mon.get<>(UartCommandGetVersion()) << std::endl;
            break;
        case 'x':
            std::cout << mon.get<>(UartCommandGetStatistics()) << std::endl;
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
                argv[i+1][0] = '\0';
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
