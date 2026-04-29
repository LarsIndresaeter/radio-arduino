
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
#include <version.h>

using namespace std::chrono_literals;

void print_usage()
{
    std::cout << "raduino-rf-scan" << std::endl;
    std::cout << "           -a : address filter" << std::endl;
    std::cout << "           -j : json print" << std::endl;
    std::cout << "           -d : decode and print (default on)" << std::endl;
    std::cout << "           -D : decode and print (turn off)" << std::endl;
    std::cout << "           -f : fields summary (default on)" << std::endl;
    std::cout << "           -F : fields summary (turn off)" << std::endl;
    std::cout << "           -s : stream operator print" << std::endl;
    std::cout << "           -t : timestamp print (default on)" << std::endl;
    std::cout << "           -T : timestamp print (turn off)" << std::endl;
    std::cout << "           -h : print this text" << std::endl;
}

void formatAndPrintFields(COMMANDS::SCAN_FOR_ADVERTISEMENT::response_t payload)
{
    std::cout << std::setw(12) << std::to_string(payload.getId()) << " | ";
    std::cout << std::setw(4) << std::to_string(payload.getFlags()) << " | ";
    std::cout << std::setw(5) << std::to_string(payload.getSequence_number()) << " | ";
    // std::cout << result.getData();
    for (uint8_t i = 0; i < 16; i++) {
        std::cout << std::setw(3) << std::to_string(payload.data[i]) << " ";
    }
    std::cout << " |";
}

bool decodeAndPrint(COMMANDS::SCAN_FOR_ADVERTISEMENT::response_t responseStruct)
{
    bool retval = false;
    uint32_t nodeAddress = responseStruct.getId();

    if (nodeAddress != 0) {
        uint8_t OI = responseStruct.data[0];
        uint8_t OL = responseStruct.data[1];
        if (OI != 0) {
            std::vector<uint8_t> advertisementDataVector;
            advertisementDataVector.push_back(0xFE);
            advertisementDataVector.push_back(0xED);
            advertisementDataVector.push_back(0x00);
            advertisementDataVector.push_back(OI + 4);

            for (uint8_t i = 0; i < OL + 2; i++) {
                advertisementDataVector.push_back(responseStruct.data[i]);
            }

            if (static_cast<COMMANDS::OI>(OI) == COMMANDS::OI::QUADRATURE_ENCODER) {
                RaduinoCommandQuadratureEncoder subscriptionObject;
                subscriptionObject.setResponse(advertisementDataVector);
                subscriptionObject.setResponseTimeUs(0);
                subscriptionObject.setReplyStatus(RaduinoCommandBase::ReplyStatus::Complete);

                std::string jsonResponse = subscriptionObject.getJson();
                std::cout << jsonResponse;
                retval = true;
            }

            if (static_cast<COMMANDS::OI>(OI) == COMMANDS::OI::GPIO) {
                RaduinoCommandGpio subscriptionObject;
                subscriptionObject.setResponse(advertisementDataVector);
                subscriptionObject.setResponseTimeUs(0);
                subscriptionObject.setReplyStatus(RaduinoCommandBase::ReplyStatus::Complete);

                std::string jsonResponse = subscriptionObject.getJson();
                std::cout << jsonResponse;
                retval = true;
            }

            if (static_cast<COMMANDS::OI>(OI) == COMMANDS::OI::VCC) {
                RaduinoCommandVcc subscriptionObject;
                subscriptionObject.setResponse(advertisementDataVector);
                subscriptionObject.setResponseTimeUs(0);
                subscriptionObject.setReplyStatus(RaduinoCommandBase::ReplyStatus::Complete);

                std::string jsonResponse = subscriptionObject.getJson();
                std::cout << jsonResponse;
                retval = true;
            }
        }
    }
    return (retval);
}

void scan(
    monitor& mon,
    uint8_t radioAddress,
    bool printJson,
    bool printStd,
    bool printTimestamp,
    bool printFields,
    bool printRaw,
    bool printDecoded)
{
    if (printTimestamp) {
        std::cout << "time received                 | ";
    }
    if (printFields) {
        std::cout << "address      | ";
        std::cout << "flag | ";
        std::cout << "seqNo | ";
        std::cout << "data                                                             | ";
    }
    if (printDecoded) {
        std::cout << "decoded object from advertisement data";
    }
    if (printRaw) {
        std::cout << "raw data";
    }
    std::cout << std::endl;

    while (true) {
        auto result = mon.get<>(RaduinoCommandScanForAdvertisement(radioAddress, 10000), 12000ms);

        if (result.responseStruct().getId() > 0) {
            if (printTimestamp) {
                uint64_t timestamp = result.getTimeStamp() + result.getResponseTimeUs();
                auto tp
                    = std::chrono::time_point<std::chrono::system_clock>(std::chrono::microseconds(timestamp * 1000));
                std::cout << std::format("{:%Y-%m-%d %H:%M:%S} | ", tp);
            }

            COMMANDS::SCAN_FOR_ADVERTISEMENT::response_t payload = result.responseStruct();

            if (printFields) {
                formatAndPrintFields(payload);
            }

            if (printDecoded) {
                if (!decodeAndPrint(payload)) {
                    if (!printFields) {
                        // could not decode, make sure we print something
                        formatAndPrintFields(payload);
                    }
                }
            }

            if (printRaw) {
                std::vector<uint8_t> raw = result.getResponse();
                for (uint8_t r : raw) {
                    std::cout << std::to_string(r) << " ";
                }
            }

            if (printStd) {
                std::cout << result << std::endl;
            }

            if (printJson) {
                std::cout << result.getJson() << std::endl;
            }
            std::cout << std::endl;
        }
    }
}

void parseOpt(int argc, char* argv[], monitor& mon, LinuxCryptoHandler& cryptoHandler)
{
    char option = 0;
    uint32_t radioAddress = 0;
    bool printJson = false;
    bool printStd = false;
    bool printTimestamp = true;
    bool printFields = true;
    bool printRaw = false;
    bool printDecoded = true;

    while ((option = getopt(argc, argv, "a:dDfFjrstTh")) != -1) {
        switch (option) {
        case 'a':
            radioAddress = atoi(optarg);
            break;
        case 'd':
            printDecoded = true;
            break;
        case 'D':
            printDecoded = false;
            break;
        case 'j':
            printJson = true;
            break;
        case 'f':
            printFields = true;
            break;
        case 'F':
            printFields = false;
            break;
        case 'r':
            printRaw = true;
            break;
        case 's':
            printStd = true;
            break;
        case 't':
            printTimestamp = true;
            break;
        case 'T':
            printTimestamp = false;
            break;
        case 'h':
            print_usage();
            break;
        }
    }

    scan(mon, radioAddress, printJson, printStd, printTimestamp, printFields, printRaw, printDecoded);

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
