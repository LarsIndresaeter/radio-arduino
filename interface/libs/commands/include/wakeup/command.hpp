#pragma once

#include <common/uartCommandBase.hpp>

class UartCommandWakeup : public UartCommandBase {
public:
    UartCommandWakeup(bool wakeup_if_data_flag_is_set)
        : UartCommandBase(
            static_cast<uint8_t>(COMMANDS::OI::WAKEUP),
            COMMANDS::WAKEUP::COMMAND_LENGTH)
    {
        if (wakeup_if_data_flag_is_set) {
            m_payload.at(offsetof(COMMANDS::WAKEUP::command_t, check_attention_flag))
                = 1;
        }
        else {
            m_payload.at(offsetof(COMMANDS::WAKEUP::command_t, check_attention_flag))
                = 0;
        }
    };

    void print(std::ostream& out) const override
    {
        COMMANDS::WAKEUP::response_t response(
            (uint8_t*)&m_response.data()[4]);
        out << "WAKEUP          : ";

        if(response.status == 1)
        {
            out << "OK";

            out << " (attention=" << std::to_string(response.attention) << ")";
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

