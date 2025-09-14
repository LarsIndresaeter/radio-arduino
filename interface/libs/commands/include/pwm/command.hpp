#pragma once

#include <common/uartCommandBase.hpp>

class UartCommandPwm : public UartCommandBase {
public:
    UartCommandPwm(uint8_t port, uint8_t pin, uint8_t value)
        : UartCommandBase(
            static_cast<uint8_t>(COMMANDS::OI::PWM),
            COMMANDS::PWM::COMMAND_LENGTH)
    {
        COMMANDS::PWM::command_t command;

        m_payload.at(offsetof(COMMANDS::PWM::command_t, port)) = port;

        m_payload.at(offsetof(COMMANDS::PWM::command_t, pin)) = pin;

        m_payload.at(offsetof(COMMANDS::PWM::command_t, value)) = value;

    };

    void printResponse(std::ostream& out, COMMANDS::PWM::response_t response) const
    {
        out << "PWM   : ";
        UartCommandBase::print(out);
    };

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::PWM::RESPONSE_LENGTH + 4)) {
            COMMANDS::PWM::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        } else
        {
            std::cout << "PWM: insufficient data" << std::endl;
        }
    };

    COMMANDS::PWM::response_t responseStruct()
    {
        return { (uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH] };
    };
};

