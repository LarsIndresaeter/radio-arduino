#pragma once

#include <common/uartCommandBase.hpp>

class UartCommandWakeup : public UartCommandBase {
public:
    UartCommandWakeup(uint8_t checkAttentionFlag)
        : UartCommandBase(
            static_cast<uint8_t>(COMMANDS::OI::WAKEUP),
            COMMANDS::WAKEUP::COMMAND_LENGTH)
    {
        COMMANDS::WAKEUP::command_t command;

        m_payload.at(offsetof(COMMANDS::WAKEUP::command_t, checkAttentionFlag)) = checkAttentionFlag;

    };

    void printResponse(std::ostream& out, COMMANDS::WAKEUP::response_t response) const
    {
        out << "WAKEUP   : ";

        if(response.status == 1)
        {
            out << "OK";

            out << " (attention=" << std::to_string(response.attention) << ")";
        }
        else
        {
            out << "FAILED";
        }
    }

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::WAKEUP::RESPONSE_LENGTH + 4)) {
            COMMANDS::WAKEUP::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        } else
        {
            std::cout << "WAKEUP: insufficient data" << std::endl;
        }
    };

    COMMANDS::WAKEUP::response_t responseStruct()
    {
        return { (uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH] };
    };
};

