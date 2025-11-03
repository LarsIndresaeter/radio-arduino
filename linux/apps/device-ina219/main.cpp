
#include <chrono>
#include <cinttypes>
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

void print_usage()
{
    std::cout << "raduino-device-ina219" << std::endl;
    std::cout << "       -K : encrypt command with transport key" << std::endl;
    std::cout << "       -N : wakeup node address" << std::endl;
    std::cout << "       -s : read ina219 on gateway" << std::endl;
    std::cout << "       -s : read ina219 on node" << std::endl;
    std::cout << "       -n : ina219 stats for <N> seconds on gateway" << std::endl;
    std::cout << "       -h : print this text" << std::endl;
}

int timeMs()
{
    using namespace std::chrono;
    return (duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count()) % 1000;
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
    uint8_t spiRegister = 0;
    uint8_t radioAddress = 0;

    while ((option = getopt(argc, argv, "K:N:sSn:h")) != -1) {
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

        case 's': {
            auto result = mon.get<>(RaduinoCommandIna219());

            float current = ((int16_t)result.responseStruct().getCurrent()) * 0.001;
            float voltage = ((int16_t)(result.responseStruct().getVoltage() >> 3)) * 0.004;

            std::cout << result;
            std::cout << " (float) I=" << std::to_string(current);
            std::cout << ",   V=" << std::to_string(voltage) << std::endl;
        } break;
        case 'S':
            std::cout << mon.getRadio<>(RaduinoCommandIna219()) << std::endl;
            break;
        case 'n':
            readCurrentAndVoltage(mon, atoi(optarg));
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
