#pragma once

#include <common/uartCommandBase.hpp>

class UartCommandRandom : public UartCommandBase {
public:
    UartCommandRandom()
        : UartCommandBase(
            static_cast<uint8_t>(COMMANDS::OI::RANDOM),
            COMMANDS::RANDOM::COMMAND_LENGTH) {};

    void print(std::ostream& out) const override
    {
        out << "RANDOM        : ";
        UartCommandBase::print(out);
    };

    COMMANDS::RANDOM::response_t responseStruct()
    {
        return {(uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH]};
    };
};

