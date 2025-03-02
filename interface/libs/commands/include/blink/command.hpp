#pragma once

#include <common/uartCommandBase.hpp>

class UartCommandBlink : public UartCommandBase {
public:
    UartCommandBlink()
        : UartCommandBase(
            static_cast<uint8_t>(COMMANDS::OI::BLINK),
            COMMANDS::BLINK::COMMAND_LENGTH) {};

    void print(std::ostream& out) const override
    {
        uint8_t c;
        out << "BLINK         : ";
        UartCommandBase::print(out);
    };

    COMMANDS::BLINK::response_t responseStruct()
    {
        return {(uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH]};
    };
};

