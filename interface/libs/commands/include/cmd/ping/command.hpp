#pragma once
// This file is generated with the script: `interface/libs/commands/generate.py`

#include <common/uartCommandBase.hpp>

class UartCommandPing : public UartCommandBase {
public:
    UartCommandPing()
        : UartCommandBase(
              static_cast<uint8_t>(COMMANDS::OI::PING),
              COMMANDS::PING::COMMAND_LENGTH)
    {
        COMMANDS::PING::command_t command;

    };

    void printResponse(std::ostream& out, COMMANDS::PING::response_t response) const
    {
        out << "PING                   : ";
    }


    std::string getCommandName() { return "ping";}

    std::string getJson() {
        std::string json;
        json.append("{");
        json.append("\"name\":");
        json.append("\"ping\", ");
        json.append("\"timestamp\":");
        json.append(std::to_string(getTimeStamp()));
        json.append(", ");
        json.append("}");
        return(json);
    };

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::PING::RESPONSE_LENGTH + 4)) {
            COMMANDS::PING::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        }
        else {
            std::cout << "PING: insufficient data" << std::endl;
        }
    };

    COMMANDS::PING::response_t responseStruct()
    {
        COMMANDS::PING::response_t response;

        if (m_responsePayload.size() >= sizeof(response)) {
            return { (uint8_t*)&m_responsePayload[0] };
        }

        return (response);
    };
};

