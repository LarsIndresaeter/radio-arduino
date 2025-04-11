#pragma once

#include <common/uartCommandBase.hpp>

class UartCommandSetNodeAddress : public UartCommandBase {
public:
    UartCommandSetNodeAddress(uint8_t address)
        : UartCommandBase(
            static_cast<uint8_t>(COMMANDS::OI::SET_NODE_ADDRESS),
            COMMANDS::SET_NODE_ADDRESS::COMMAND_LENGTH)
    {
        m_payload.at(offsetof(COMMANDS::SET_NODE_ADDRESS::command_t, node_address)) = address;
    };

    void print(std::ostream& out) const override
    {
        COMMANDS::SET_NODE_ADDRESS::response_t response(
            (uint8_t*)&m_response.data()[4]);
        out << "SET_NODE_ADDRESS           : ";

        if(response.status == 1)
        {
            out << "OK";
        }
        else
        {
            out << "FAILED";
        }
    };

    COMMANDS::SET_NODE_ADDRESS::response_t responseStruct()
    {
        return {(uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH]};
    };
};

