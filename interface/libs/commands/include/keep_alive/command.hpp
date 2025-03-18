#pragma once

#include <common/uartCommandBase.hpp>

class UartCommandKeepAlive : public UartCommandBase {
public:
    UartCommandKeepAlive(uint8_t time)
        : UartCommandBase(
            static_cast<uint8_t>(COMMANDS::OI::KEEP_ALIVE),
            COMMANDS::KEEP_ALIVE::COMMAND_LENGTH)
    {
        m_payload.at(offsetof(COMMANDS::KEEP_ALIVE::command_t, time)) = time;
    };

    void print(std::ostream& out) const override
    {
        COMMANDS::SET_SLAVE_ADDRESS::response_t response(
            (uint8_t*)&m_response.data()[4]);
        out << "KEEP_ALIVE           : ";

        if(response.status == 1)
        {
            out << "OK";
        }
        else
        {
            out << "FAILED";
        }
    };

    COMMANDS::KEEP_ALIVE::response_t responseStruct()
    {
        return {(uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH]};
    };
};

