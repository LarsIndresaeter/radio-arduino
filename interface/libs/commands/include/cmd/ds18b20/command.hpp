#pragma once
// This file is generated with the script: `interface/libs/commands/generate.py`

#include <common/uartCommandBase.hpp>

class UartCommandDs18b20 : public UartCommandBase {
public:
    UartCommandDs18b20()
        : UartCommandBase(
              static_cast<uint8_t>(COMMANDS::OI::DS18B20),
              COMMANDS::DS18B20::COMMAND_LENGTH)
    {
        COMMANDS::DS18B20::command_t command;
    };

    void printResponse(std::ostream& out, COMMANDS::DS18B20::response_t response) const
    {
        out << "DS18B20                : ";
        out << " temperature=" << static_cast<int>(response.getTemperature());
    }

    std::string getCommandName() { return "ds18b20"; }

    std::string getJson()
    {
        std::string json;
        json.append("{");
        json.append("\"name\":");
        json.append("\"ds18b20\", ");
        json.append("\"timestamp\":");
        json.append(std::to_string(getTimeStamp()));
        json.append(", ");
        json.append("\"temperature\":");
        json.append(std::to_string(responseStruct().getTemperature()));
        json.append("}");
        return (json);
    };

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::DS18B20::RESPONSE_LENGTH + 4)) {
            COMMANDS::DS18B20::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        }
        else {
            std::cout << "DS18B20: insufficient data" << std::endl;
        }
    };

    COMMANDS::DS18B20::response_t responseStruct()
    {
        COMMANDS::DS18B20::response_t response;

        if (m_responsePayload.size() >= sizeof(response)) {
            return { (uint8_t*)&m_responsePayload[0] };
        }

        return (response);
    };
};

