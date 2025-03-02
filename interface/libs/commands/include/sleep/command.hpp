#pragma once

#include <common/uartCommandBase.hpp>

class UartCommandSleep : public UartCommandBase {
public:
    UartCommandSleep(uint32_t delay)
        : UartCommandBase(
            static_cast<uint8_t>(COMMANDS::OI::SLEEP),
            COMMANDS::SLEEP::COMMAND_LENGTH)
    {
        m_payload.at(offsetof(COMMANDS::SLEEP::command_t, delay_0))
            = static_cast<uint8_t>(delay >> 24);
        m_payload.at(offsetof(COMMANDS::SLEEP::command_t, delay_1))
            = static_cast<uint8_t>(delay >> 16);
        m_payload.at(offsetof(COMMANDS::SLEEP::command_t, delay_2))
            = static_cast<uint8_t>(delay >> 8);
        m_payload.at(offsetof(COMMANDS::SLEEP::command_t, delay_3))
            = static_cast<uint8_t>(delay);
    };

    void print(std::ostream& out) const override
    {
        COMMANDS::SLEEP::response_t response(
            (uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH]);

        out << "SLEEP          : ";
        if (response.status == 0) {
            out << "ERROR";
        }
        else {
            out << "OK";
        }
    };

    COMMANDS::SLEEP::response_t responseStruct()
    {
        return { (uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH] };
    };
};

