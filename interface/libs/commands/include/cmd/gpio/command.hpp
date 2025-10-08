#pragma once
// This file is generated with the script: `interface/libs/commands/generate.py`

#include <common/uartCommandBase.hpp>

class UartCommandGpio : public UartCommandBase {
public:
    UartCommandGpio()
        : UartCommandBase(
              static_cast<uint8_t>(COMMANDS::OI::GPIO),
              COMMANDS::GPIO::COMMAND_LENGTH)
    {
        COMMANDS::GPIO::command_t command;
    };

    void printResponse(std::ostream& out, COMMANDS::GPIO::response_t response) const
    {
        out << "GPIO                   : ";
        out << " portB=" << static_cast<int>(response.getPortb());
        out << " portC=" << static_cast<int>(response.getPortc());
        out << " portD=" << static_cast<int>(response.getPortd());
    }

    std::string getCommandName() { return "gpio"; }

    std::string getJson()
    {
        std::string json;
        json.append("{");
        json.append("\"name\":");
        json.append("\"gpio\", ");
        json.append("\"timestamp\":");
        json.append(std::to_string(getTimeStamp()));
        json.append(", ");
        json.append("\"portB\":");
        json.append(std::to_string(responseStruct().getPortb()));
        json.append(", ");
        json.append("\"portC\":");
        json.append(std::to_string(responseStruct().getPortc()));
        json.append(", ");
        json.append("\"portD\":");
        json.append(std::to_string(responseStruct().getPortd()));
        json.append("}");
        return (json);
    };

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::GPIO::RESPONSE_LENGTH + 4)) {
            COMMANDS::GPIO::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        }
        else {
            std::cout << "GPIO: insufficient data" << std::endl;
        }
    };

    COMMANDS::GPIO::response_t responseStruct()
    {
        COMMANDS::GPIO::response_t response;

        if (m_responsePayload.size() >= sizeof(response)) {
            return { (uint8_t*)&m_responsePayload[0] };
        }

        return (response);
    };
};

