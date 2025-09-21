#pragma once
// This file is generated with the script: `interface/libs/commands/generate.py`

#include <common/uartCommandBase.hpp>

class UartCommandTimer : public UartCommandBase {
public:
    UartCommandTimer()
        : UartCommandBase(
            static_cast<uint8_t>(COMMANDS::OI::TIMER),
            COMMANDS::TIMER::COMMAND_LENGTH)
    {
        COMMANDS::TIMER::command_t command;

    };

    void printResponse(std::ostream& out, COMMANDS::TIMER::response_t response) const
    {
        out << "TIMER                  : ";
        out << " pulseWidth=" << static_cast<int>(response.getPulsewidth());
    }

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::TIMER::RESPONSE_LENGTH + 4)) {
            COMMANDS::TIMER::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        } else
        {
            std::cout << "TIMER: insufficient data" << std::endl;
        }
    };

    COMMANDS::TIMER::response_t responseStruct()
    {
        return { (uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH] };
    };
};

