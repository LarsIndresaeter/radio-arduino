#pragma once

#include <common/uartCommandBase.hpp>

class UartCommandKeepAlive : public UartCommandBase {
public:
    UartCommandKeepAlive(uint8_t time)
        : UartCommandBase(
            static_cast<uint8_t>(COMMANDS::OI::KEEP_ALIVE),
            COMMANDS::KEEP_ALIVE::COMMAND_LENGTH)
    {
        COMMANDS::KEEP_ALIVE::command_t command;

        m_payload.at(offsetof(COMMANDS::KEEP_ALIVE::command_t, time)) = time;

    };

    void printResponse(std::ostream& out, COMMANDS::KEEP_ALIVE::response_t response) const
    {
        out << "KEEP_ALIVE   : ";

        if(response.status == 1)
        {
            out << "OK";
        }
        else
        {
            out << "FAILED";
        }
    };

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::KEEP_ALIVE::RESPONSE_LENGTH + 4)) {
            COMMANDS::KEEP_ALIVE::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        } else
        {
            std::cout << "KEEP_ALIVE: insufficient data" << std::endl;
        }
    };

    COMMANDS::KEEP_ALIVE::response_t responseStruct()
    {
        return { (uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH] };
    };
};

