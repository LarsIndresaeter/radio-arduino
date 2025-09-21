#pragma once
// This file is generated with the script: `interface/libs/commands/generate.py`

#include <common/uartCommandBase.hpp>

class UartCommandSpiRead : public UartCommandBase {
public:
    UartCommandSpiRead(uint8_t reg, uint8_t length)
        : UartCommandBase(
              static_cast<uint8_t>(COMMANDS::OI::SPI_READ),
              COMMANDS::SPI_READ::COMMAND_LENGTH)
    {
        COMMANDS::SPI_READ::command_t command;

        m_payload.at(offsetof(COMMANDS::SPI_READ::command_t, reg)) = reg;

        m_payload.at(offsetof(COMMANDS::SPI_READ::command_t, length)) = length;

    };

    void printResponse(std::ostream& out, COMMANDS::SPI_READ::response_t response) const
    {
        out << "SPI_READ               : ";
        out << " reg=" << static_cast<int>(response.getReg());
        out << " length=" << static_cast<int>(response.getLength());
        out << " data=[ ";
        out << std::setfill('0') << std::hex << std::uppercase;
        for (uint8_t i = 0; i < 32; i++) {
            out << std::setw(2) << static_cast<int>(response.data[i]) << " ";
        }
        out << "]";
        out << std::dec;
    }

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::SPI_READ::RESPONSE_LENGTH + 4)) {
            COMMANDS::SPI_READ::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        }
        else {
            std::cout << "SPI_READ: insufficient data" << std::endl;
        }
    };

    COMMANDS::SPI_READ::response_t responseStruct()
    {
        COMMANDS::SPI_READ::response_t response;

        if (m_responsePayload.size() >= sizeof(response)) {
            return { (uint8_t*)&m_responsePayload[0] };
        }

        return (response);
    };
};

