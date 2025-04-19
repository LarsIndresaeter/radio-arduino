#pragma once

#include <common/uartCommandBase.hpp>

class UartCommandPing : public UartCommandBase {
public:
    UartCommandPing()
        : UartCommandBase(
            static_cast<uint8_t>(COMMANDS::OI::PING),
            COMMANDS::PING::COMMAND_LENGTH) {};

    void print(std::ostream& out) const override
    {
        out << "PING            : ";
        if (m_replyStatus == ReplyStatus::Complete) {
            out << "OK";

            out << " (" << std::to_string(m_responseTimeUs/1000);
            out << "." << std::to_string((m_responseTimeUs%1000)/100);
            out << " ms)";
        }
        else {
            out << "FAILED";
        }
    };

    COMMANDS::PING::response_t responseStruct()
    {
        return((uint8_t*){});
    }
};

