#pragma once

#include <common/uartCommandBase.hpp>

class UartCommandSetSlaveAddress : public UartCommandBase {
public:
    UartCommandSetSlaveAddress(uint8_t address)
        : UartCommandBase(
            static_cast<uint8_t>(COMMANDS::OI::SET_SLAVE_ADDRESS),
            COMMANDS::SET_SLAVE_ADDRESS::COMMAND_LENGTH)
    {
        m_payload.at(offsetof(COMMANDS::SET_SLAVE_ADDRESS::command_t, slave_address)) = address;
    };

    void print(std::ostream& out) const override
    {
        out << "SET_SLAVE_ADDRESS           : ";
        UartCommandBase::print(out);
    };

    COMMANDS::SET_SLAVE_ADDRESS::response_t responseStruct()
    {
        return {(uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH]};
    };
};

