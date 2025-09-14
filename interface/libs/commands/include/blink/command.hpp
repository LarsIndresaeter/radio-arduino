#pragma once

#include <common/uartCommandBase.hpp>

class UartCommandBlink : public UartCommandBase {
public:
    UartCommandBlink()
        : UartCommandBase(
            static_cast<uint8_t>(COMMANDS::OI::BLINK),
            COMMANDS::BLINK::COMMAND_LENGTH)
    {
        COMMANDS::BLINK::command_t command;

    };

    void printResponse(std::ostream& out, COMMANDS::BLINK::response_t response) const
    {
        out << "BLINK   : ";
        uint8_t c;
        out << "BLINK         : ";
        UartCommandBase::print(out);
    };

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::BLINK::RESPONSE_LENGTH + 4)) {
            COMMANDS::BLINK::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        } else
        {
            std::cout << "BLINK: insufficient data" << std::endl;
        }
    };

    COMMANDS::BLINK::response_t responseStruct()
    {
        return { (uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH] };
    };
};

