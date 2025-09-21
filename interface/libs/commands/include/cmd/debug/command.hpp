#pragma once
// This file is generated with the script: `interface/libs/commands/generate.py`

#include <common/uartCommandBase.hpp>

class UartCommandDebug : public UartCommandBase {
public:
    UartCommandDebug()
        : UartCommandBase(
            static_cast<uint8_t>(COMMANDS::OI::DEBUG),
            COMMANDS::DEBUG::COMMAND_LENGTH)
    {
        COMMANDS::DEBUG::command_t command;

    };

    void printResponse(std::ostream& out, COMMANDS::DEBUG::response_t response) const
    {
        out << "DEBUG                  : ";
        out << " data=[ ";
        out << std::setfill('0') << std::hex << std::uppercase;
        for(uint8_t i=0; i<32; i++)
        {
            out << std::setw(2) << static_cast<int>(response.data[i]) << " ";
        }
        out << "]";
        out << std::dec;
    }

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::DEBUG::RESPONSE_LENGTH + 4)) {
            COMMANDS::DEBUG::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        } else
        {
            std::cout << "DEBUG: insufficient data" << std::endl;
        }
    };

    COMMANDS::DEBUG::response_t responseStruct()
    {
        return { (uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH] };
    };
};

