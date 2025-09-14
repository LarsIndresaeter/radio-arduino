#pragma once

#include <common/uartCommandBase.hpp>

class UartCommandHotp : public UartCommandBase {
public:
    UartCommandHotp()
        : UartCommandBase(
            static_cast<uint8_t>(COMMANDS::OI::HOTP),
            COMMANDS::HOTP::COMMAND_LENGTH)
    {
        COMMANDS::HOTP::command_t command;

    };

    void printResponse(std::ostream& out, COMMANDS::HOTP::response_t response) const
    {
        out << "HOTP   : ";
        uint8_t c;
        UartCommandBase::print(out);
    };

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::HOTP::RESPONSE_LENGTH + 4)) {
            COMMANDS::HOTP::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        } else
        {
            std::cout << "HOTP: insufficient data" << std::endl;
        }
    };

    COMMANDS::HOTP::response_t responseStruct()
    {
        return { (uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH] };
    };
};

