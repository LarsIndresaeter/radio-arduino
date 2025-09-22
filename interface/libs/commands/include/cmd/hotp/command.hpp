#pragma once
// This file is generated with the script: `interface/libs/commands/generate.py`

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
        out << "HOTP                   : ";
        out << " data=[ ";
        out << std::setfill('0') << std::hex << std::uppercase;
        for (uint8_t i = 0; i < 20; i++) {
            out << std::setw(2) << static_cast<int>(response.data[i]) << " ";
        }
        out << "]";
        out << std::dec;
    }

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::HOTP::RESPONSE_LENGTH + 4)) {
            COMMANDS::HOTP::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        }
        else {
            std::cout << "HOTP: insufficient data" << std::endl;
        }
    };

    COMMANDS::HOTP::response_t responseStruct()
    {
        COMMANDS::HOTP::response_t response;

        if (m_responsePayload.size() >= sizeof(response)) {
            return { (uint8_t*)&m_responsePayload[0] };
        }

        return (response);
    };
};

