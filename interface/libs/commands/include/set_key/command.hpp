#pragma once

#include <common/uartCommandBase.hpp>

class UartCommandSetKey : public UartCommandBase {
public:
    UartCommandSetKey(uint8_t key_id, std::vector<uint8_t> data)
        : UartCommandBase(
            static_cast<uint8_t>(COMMANDS::OI::SET_KEY),
            COMMANDS::SET_KEY::COMMAND_LENGTH)
    {
        m_payload.at(offsetof(COMMANDS::SET_KEY::command_t, key_id))
            = key_id;

        for (int i = 0; i < data.size() & i < 16; i++) {
            m_payload.at(offsetof(COMMANDS::SET_KEY::command_t, key_value) + i)
                = data.at(i);
        }
    };

    void print(std::ostream& out) const override
    {
        out << "SET_KEY       : ";
        UartCommandBase::print(out);
    };

    COMMANDS::SET_KEY::response_t responseStruct()
    {
        return { (uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH] };
    }
};

