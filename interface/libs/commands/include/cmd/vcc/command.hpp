#pragma once
// This file is generated with the script: `interface/libs/commands/generate.py`

#include <common/uartCommandBase.hpp>

class UartCommandVcc : public UartCommandBase {
public:
    UartCommandVcc()
        : UartCommandBase(
              static_cast<uint8_t>(COMMANDS::OI::VCC),
              COMMANDS::VCC::COMMAND_LENGTH)
    {
        COMMANDS::VCC::command_t command;

    };

    void printResponse(std::ostream& out, COMMANDS::VCC::response_t response) const
    {
        out << "VCC                    : ";
        out << " vcc=" << static_cast<int>(response.getVcc());
    }


    std::string getCommandName() { return "vcc";}

    std::string getJson() {
        std::string json;
        json.append("{");
        json.append("\"timestamp\":");
        json.append(std::to_string(getTimeStamp()));
        json.append("\"name\":");
        json.append("\"vcc\", ");
        json.append(", ");
        json.append("\"vcc\":");
        json.append(std::to_string(responseStruct().getVcc()));
        json.append("}");
        return(json);
    };

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::VCC::RESPONSE_LENGTH + 4)) {
            COMMANDS::VCC::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        }
        else {
            std::cout << "VCC: insufficient data" << std::endl;
        }
    };

    COMMANDS::VCC::response_t responseStruct()
    {
        COMMANDS::VCC::response_t response;

        if (m_responsePayload.size() >= sizeof(response)) {
            return { (uint8_t*)&m_responsePayload[0] };
        }

        return (response);
    };
};

