#pragma once
// This file is generated with the script: `interface/libs/commands/generate.py`

#include <common/uartCommandBase.hpp>

class UartCommandWakeup : public UartCommandBase {
public:
    UartCommandWakeup(uint8_t checkAttentionFlag)
        : UartCommandBase(
              static_cast<uint8_t>(COMMANDS::OI::WAKEUP),
              COMMANDS::WAKEUP::COMMAND_LENGTH)
    {
        COMMANDS::WAKEUP::command_t command;

        m_payload.at(offsetof(COMMANDS::WAKEUP::command_t, checkAttentionFlag)) = checkAttentionFlag;

    };

    void printResponse(std::ostream& out, COMMANDS::WAKEUP::response_t response) const
    {
        out << "WAKEUP                 : ";
        out << " attention=" << static_cast<int>(response.getAttention());
    }


    std::string getCommandName() { return "wakeup";}

    std::string getJson() {
        std::string json;
        json.append("{");
        json.append("\"name\":");
        json.append("\"wakeup\", ");
        json.append("\"timestamp\":");
        json.append(std::to_string(getTimeStamp()));
        json.append(", ");
        json.append("\"attention\":");
        json.append(std::to_string(responseStruct().getAttention()));
        json.append("}");
        return(json);
    };

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::WAKEUP::RESPONSE_LENGTH + 4)) {
            COMMANDS::WAKEUP::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        }
        else {
            std::cout << "WAKEUP: insufficient data" << std::endl;
        }
    };

    COMMANDS::WAKEUP::response_t responseStruct()
    {
        COMMANDS::WAKEUP::response_t response;

        if (m_responsePayload.size() >= sizeof(response)) {
            return { (uint8_t*)&m_responsePayload[0] };
        }

        return (response);
    };
};

