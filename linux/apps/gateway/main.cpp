
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

int timeMs()
{
    using namespace std::chrono;
    return (duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count()) % 1000;
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
    std::cout << "       -M : ina219 power monitor" << std::endl;
    std::cout << "       -N : ina219 power monitor, statistic per second for "
                 "<N> seconds"
              << std::endl;
    std::cout << "       -X : ds18b20 temperature sensor" << std::endl;
    std::cout << "       -Z : set device name" << std::endl;
    std::cout << "       -z : get device name" << std::endl;
    std::cout << "       -a : get device version" << std::endl;
    std::cout << "       -x : get statistics" << std::endl;
    std::cout << "       -W : WS2812B <string>" << std::endl;
    std::cout << "       -F : Read servo PWM signal and display on ws2812b ring" << std::endl;
    std::cout << "       -U : radio uart command <s> send, <r> receive" << std::endl;
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
    auto ina219 = mon.get<>(RaduinoCommandIna219());
    std::vector<float> currentData, voltageData;
    int16_t intval = 0;
    float currentMin, currentMax, current, stdev;
    float voltageMin, voltageMax, voltage;
    int time, timePrev;
    int loopCounter = 0;

    std::cout << "timestamp          , sam, currentMin  , currentAverage , "
                 "currentMax  , currentStd, minVoltage, averageVoltage, maxVoltage"
              << std::endl;

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
            ina219 = mon.get<>(RaduinoCommandIna219());
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
    uint16_t i2cDeviceOffset = 0;
    uint8_t i2cDeviceAddress = 0b10100000;

    while ((option = getopt(argc, argv, "P:DBSHCs:RVvhtTgGMN:XE:K:b:r:Z:zW:L:FJU:jpaxnu")) != -1) {
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
        case 'W': {
            std::vector<uint8_t> red(45);
            std::vector<uint8_t> green(45);
            std::vector<uint8_t> blue(45);
            COMMANDS::WS2812B::command_t command;
            std::string s(optarg);

            if (s.at(0) == 'a') {
                using namespace std::chrono;
                int random_variable
                    = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count() % 90 + 45;
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
                    RaduinoCommandWs2812b ws2812b(red, green, blue);
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
        case 'F': {
            while (true) {
                auto r = mon.get<>(RaduinoCommandTimer());

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

                    RaduinoCommandWs2812b ws2812b(red, green, blue);
                    mon.get<>(ws2812b);
                }
            }
        }
        case 'D':
            std::cout << mon.get<>(RaduinoCommandDebug()) << std::endl;
            break;
        case 'p':
            std::cout << mon.get<>(RaduinoCommandPing()) << std::endl;
            break;
        case 'j':
            std::cout << mon.get<>(RaduinoCommandVcc()) << std::endl;
            break;
        case 'M':
            std::cout << mon.get<>(RaduinoCommandIna219()) << std::endl;
            break;
        case 'N':
            readCurrentAndVoltage(mon, atoi(optarg));
            break;
        case 'X':
            std::cout << mon.get<>(RaduinoCommandDs18b20()) << std::endl;
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
