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
        COMMANDS::SET_NODE_ADDRESS::response_t response(
            (uint8_t*)&m_response.data()[4]);
        out << "WAKEUP          : ";

        if(response.status == 1)
        {
            out << "OK";
        }
        else
        {
            out << "FAILED";
        }
    };

    COMMANDS::WAKEUP::response_t responseStruct()
    {
        return {(uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH]};
    };
};

