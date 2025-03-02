#pragma once

#include <common/uartCommandBase.hpp>

class UartCommandSpiRead : public UartCommandBase {
public:
    UartCommandSpiRead(uint8_t reg, uint8_t length)

        : UartCommandBase(
            static_cast<uint8_t>(COMMANDS::OI::SPI_READ),
            COMMANDS::SPI_READ::COMMAND_LENGTH)
    {
        m_payload.at(offsetof(COMMANDS::SPI_READ::command_t, reg)) = reg;
        m_payload.at(offsetof(COMMANDS::SPI_READ::command_t, length)) = length;
    };

    void print(std::ostream& out) const override
    {
        COMMANDS::SPI_READ::response_t response(
            (uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH]);

        out << "SPI_READ   : ";
        out << " data[0x" << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << static_cast<int>(response.reg) << "]=[";

        for (int i = 0;
             (i < response.length) && (i < COMMANDS::SPI_READ::MAX_DATA_LENGTH);
             i++) {
            out << " 0x" << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << static_cast<int>(response.data[i]);
        }
        out << " ]";
    };

    COMMANDS::SPI_READ::response_t responseStruct()

    {
        return { (uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH] };
    };
};

