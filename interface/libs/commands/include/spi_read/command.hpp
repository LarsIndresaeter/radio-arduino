#pragma once

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
        out << "SPI_READ   : ";

        out << " data[0x" << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << static_cast<int>(response.reg) << "]=[";

        for (int i = 0;
             (i < response.length) && (i < sizeof(response.data));
             i++) {
            out << " 0x" << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << static_cast<int>(response.data[i]);
        }
        out << " ]";
    }

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::SPI_READ::RESPONSE_LENGTH + 4)) {
            COMMANDS::SPI_READ::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        } else
        {
            std::cout << "SPI_READ: insufficient data" << std::endl;
        }
    };

    COMMANDS::SPI_READ::response_t responseStruct()
    {
        return { (uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH] };
    };
};

