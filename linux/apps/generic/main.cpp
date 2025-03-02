
#ifdef USE_FEATURE_MQTT
#include "mqtt/async_client.h"
#endif

#include <chrono>
#include <cmath>
#include <commands.hpp>
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
    std::cout << "uart-tool" << std::endl;
    std::cout << "       -V : Verbose on" << std::endl;
    std::cout << "       -v : Verbose off" << std::endl;
    std::cout << "       -B : Blink command" << std::endl;
    std::cout << "       -E : EEPROM command" << std::endl;
    std::cout << "       -S : SHA1 command " << std::endl;
    std::cout << "       -H : HOTP command" << std::endl;
    std::cout << "       -A : test AES" << std::endl;
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

void parseOpt(int argc, char* argv[], monitor& mon)
{
    char option = 0;
    uint16_t i2cDeviceOffset = 0;
    uint8_t i2cDeviceAddress = 0b10100000;

    while ((option
            = getopt(argc, argv, "P:DBSHEACs:Rd:VvhtTgGi:I:o:MN:XK:Z:zW:L:FJU:j"))
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
                    mon.get<>(ws2812b);
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

            mon.get<>(ws2812b);
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
                    auto rec = mon.get<>(UartCommandNrf24l01Read(ackPayload));
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
        case 'S': {
            std::string s { "best" };
            std::vector<uint8_t> v { s.begin(), s.end() };
            std::cout << mon.get<>(UartCommandSha1(v)) << std::endl;
            compareResult(
                0xc5, mon.get<>(UartCommandSha1(v)).responseStruct().data[0]);
            compareResult(
                0xf5, mon.get<>(UartCommandSha1(v)).responseStruct().data[1]);
            compareResult(
                0xe4, mon.get<>(UartCommandSha1(v)).responseStruct().data[2]);
        } break;
        case 'L': {
            std::string s(optarg);
            std::vector<uint8_t> lcd(COMMANDS::SSD1306::STRING_LENGTH, ' ');

            for (uint8_t i = 0; i < s.size() && i < COMMANDS::SSD1306::STRING_LENGTH; i++) {
                lcd.at(i) = s.at(i);
            }

            mon.get<>(UartCommandSsd1306(2, lcd)); // second line
        } break;
        case 'H':
            std::cout << mon.get<>(UartCommandHotp()) << std::endl;
            break;
        case 'A':
            testAes(mon);
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

                uint16_t pulse_width = r.responseStruct().pulse_width_high << 8;
                pulse_width |= r.responseStruct().pulse_width_low;

                std::cout << "pulse_width=" << pulse_width << std::endl;

                if ((pulse_width > 1000) && (pulse_width < 2000)) {
                    UartCommandWs2812b ws2812b;
                    int v = (pulse_width - 1000) / 22;
                    for (int i = 0; i < v; i++) {
                        ws2812b.setLed(i, 1, 1, 1);
                    }

                    mon.get<>(ws2812b);
                }
            }
        }
        case 'D':
            std::cout << mon.get<>(UartCommandDebug()) << std::endl;
            break;
        case 'j':
            std::cout << mon.get<>(UartCommandVcc()) << std::endl;
            break;
        case 'i': {
            std::string s(optarg);
            std::vector<uint8_t> vec(s.begin(), s.end());
            std::cout << mon.get<>(
                UartCommandI2cWrite(i2cDeviceAddress, i2cDeviceOffset, vec))
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
            mon.get<>(UartCommandSetKey(COMMANDS::SET_KEY::EK, key));

            // debug
            // for (int i = 0; i < 16; i++) {
            // std::cout << mon.get<>(UartCommandEepromRead(64 + i))
            //<< std::endl;
            //}
        } break;
        case 'Z': {
            std::string s(optarg);
            std::vector<uint8_t> name;

            // read name ascii values
            for (uint8_t i = 0; i < s.size() & i < 16; i++) {
                name.push_back(s.at(i));
            }

            // set name
            mon.get<>(UartCommandSetDeviceInfo(name));

            // debug
            // for (int i = 0; i < 16; i++) {
            // std::cout << mon.get<>(UartCommandEepromRead(64 + i))
            //<< std::endl;
            //}
        } break;
        case 'z':
            std::cout << mon.get<>(UartCommandGetDeviceInfo()) << std::endl;
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

    parseOpt(argc, argv, mon);

    readerThread.join();

    return 0;
}
