#pragma once

#include <common/uartCommandBase.hpp>

class UartCommandVcc : public UartCommandBase {
public:
    UartCommandVcc()
        : UartCommandBase(
            static_cast<uint8_t>(COMMANDS::OI::VCC),
            COMMANDS::VCC::COMMAND_LENGTH)
    {
        COMMANDS::VCC::command_t command;

    };

    void printResponse(std::ostream& out, COMMANDS::VCC::response_t response) const
    {
        out << "VCC   : ";

        out << ": " << std::dec << static_cast<int>(response.getVcc()) << " mV";
    }

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::VCC::RESPONSE_LENGTH + 4)) {
            COMMANDS::VCC::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        } else
        {
            std::cout << "VCC: insufficient data" << std::endl;
        }
    };

    COMMANDS::VCC::response_t responseStruct()
    {
        return { (uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH] };
    };
};

