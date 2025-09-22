#pragma once
// This file is generated with the script: `interface/libs/commands/generate.py`

#include <common/uartCommandBase.hpp>

class UartCommandSleep : public UartCommandBase {
public:
    UartCommandSleep(uint32_t delay)
        : UartCommandBase(
              static_cast<uint8_t>(COMMANDS::OI::SLEEP),
              COMMANDS::SLEEP::COMMAND_LENGTH)
    {
        COMMANDS::SLEEP::command_t command;

        m_payload.at(offsetof(COMMANDS::SLEEP::command_t, delay) + 3) = delay>>24;
        m_payload.at(offsetof(COMMANDS::SLEEP::command_t, delay) + 2) = delay>>16;
        m_payload.at(offsetof(COMMANDS::SLEEP::command_t, delay) + 1) = delay>>8;
        m_payload.at(offsetof(COMMANDS::SLEEP::command_t, delay)) = delay;

    };

    void printResponse(std::ostream& out, COMMANDS::SLEEP::response_t response) const
    {
        out << "SLEEP                  : ";
    }

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::SLEEP::RESPONSE_LENGTH + 4)) {
            COMMANDS::SLEEP::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        }
        else {
            std::cout << "SLEEP: insufficient data" << std::endl;
        }
    };

    COMMANDS::SLEEP::response_t responseStruct()
    {
        COMMANDS::SLEEP::response_t response;

        if (m_responsePayload.size() >= sizeof(response)) {
            return { (uint8_t*)&m_responsePayload[0] };
        }

        return (response);
    };
};

