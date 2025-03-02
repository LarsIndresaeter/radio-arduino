#pragma once

#include <common/uartCommandBase.hpp>

class UartCommandHotp : public UartCommandBase {
public:
    UartCommandHotp()
        : UartCommandBase(
            static_cast<uint8_t>(COMMANDS::OI::HOTP),
            COMMANDS::HOTP::COMMAND_LENGTH) {};

    void print(std::ostream& out) const override
    {
        uint8_t c;
        out << "HOTP          : ";
        UartCommandBase::print(out);
    };

    COMMANDS::HOTP::response_t responseStruct()
    {
        return {(uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH]};
    };
};

