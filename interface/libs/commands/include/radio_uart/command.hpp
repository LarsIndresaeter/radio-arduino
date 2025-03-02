#pragma once

#include <common/uartCommandBase.hpp>

class UartCommandRadioUart : public UartCommandBase {
public:
    UartCommandRadioUart(uint8_t mode)
        : UartCommandBase(
            static_cast<uint8_t>(COMMANDS::OI::RADIO_UART),
            COMMANDS::RADIO_UART::COMMAND_LENGTH)
    {
        m_payload.at(offsetof(COMMANDS::RADIO_UART::command_t, mode))
            = mode;
    };

    void print(std::ostream& out) const override
    {
        COMMANDS::RADIO_UART::response_t response(
            (uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH]);

        out << "RADIO_UART          : status=" << static_cast<int>(response.status);
    };

    COMMANDS::RADIO_UART::response_t responseStruct()
    {
        return {(uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH]};
    };
};

