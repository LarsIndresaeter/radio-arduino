#pragma once

#include <common/uartCommandBase.hpp>

class UartCommandPwm : public UartCommandBase {
public:
    UartCommandPwm(uint8_t port, uint8_t pin, uint8_t value)
        : UartCommandBase(
            static_cast<uint8_t>(COMMANDS::OI::PWM),
            COMMANDS::PWM::COMMAND_LENGTH)
    {
        m_payload.at(offsetof(COMMANDS::PWM::command_t, port)) = port;
        m_payload.at(offsetof(COMMANDS::PWM::command_t, pin)) = pin;
        m_payload.at(offsetof(COMMANDS::PWM::command_t, value)) = value;
    };

    void print(std::ostream& out) const override
    {
        out << "PWM           : ";
        UartCommandBase::print(out);
    };

    COMMANDS::PWM::response_t responseStruct()
    {
        return {(uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH]};
    };
};

