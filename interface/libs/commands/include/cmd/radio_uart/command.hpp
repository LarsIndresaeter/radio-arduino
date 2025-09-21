#pragma once
// This file is generated with the script: `interface/libs/commands/generate.py`

#include <common/uartCommandBase.hpp>

class UartCommandRadioUart : public UartCommandBase {
public:
    UartCommandRadioUart(uint8_t mode)
        : UartCommandBase(
              static_cast<uint8_t>(COMMANDS::OI::RADIO_UART),
              COMMANDS::RADIO_UART::COMMAND_LENGTH)
    {
        COMMANDS::RADIO_UART::command_t command;

        m_payload.at(offsetof(COMMANDS::RADIO_UART::command_t, mode)) = mode;

    };

    void printResponse(std::ostream& out, COMMANDS::RADIO_UART::response_t response) const
    {
        out << "RADIO_UART             : ";
        out << " status=" << static_cast<int>(response.getStatus());
    }

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::RADIO_UART::RESPONSE_LENGTH + 4)) {
            COMMANDS::RADIO_UART::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        }
        else {
            std::cout << "RADIO_UART: insufficient data" << std::endl;
        }
    };

    COMMANDS::RADIO_UART::response_t responseStruct()
    {
        COMMANDS::RADIO_UART::response_t response;

        if (m_responsePayload.size() >= sizeof(response)) {
            return { (uint8_t*)&m_responsePayload[0] };
        }

        return (response);
    };
};

