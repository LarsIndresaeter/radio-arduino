#pragma once

#include <common/uartCommandBase.hpp>

class UartCommandWakeup : public UartCommandBase {
public:
    UartCommandWakeup()
        : UartCommandBase(
            static_cast<uint8_t>(COMMANDS::OI::WAKEUP),
            COMMANDS::WAKEUP::COMMAND_LENGTH)
    {
    };

    void print(std::ostream& out) const override
    {
        COMMANDS::WAKEUP::response_t response(
            (uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH]);

        out << "WAKEUP          : ";
        out << "status=" << static_cast<int>(response.status);
    };

    COMMANDS::WAKEUP::response_t responseStruct()
    {
        return {(uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH]};
    };
};

