#pragma once

#include <common/uartCommandBase.hpp>

class UartCommandSpiWrite : public UartCommandBase {
public:
    UartCommandSpiWrite(uint8_t reg, std::vector<uint8_t> data)
        : UartCommandBase(
            static_cast<uint8_t>(COMMANDS::OI::SPI_WRITE),
            COMMANDS::SPI_WRITE::COMMAND_LENGTH)
    {
        COMMANDS::SPI_WRITE::command_t command;

        m_payload.at(offsetof(COMMANDS::SPI_WRITE::command_t, reg)) = reg;

        if (data.size() < sizeof(command.data)) {
            m_payload.at(offsetof(COMMANDS::SPI_WRITE::command_t, length))
                = data.size();
        }
        else {
            m_payload.at(offsetof(COMMANDS::SPI_WRITE::command_t, length))
                = sizeof(command.data);
        }

        for (int i = 0; i < data.size(); i++) {
            if (i >= sizeof(command.data)) {
                std::cout << "WARNING: SPI payload truncated after "
                          << static_cast<int>(
                                 sizeof(command.data))
                          << " bytes" << std::endl;
                break;
            }
            m_payload.at(offsetof(COMMANDS::SPI_WRITE::command_t, data[0]) + i)
                = data.at(i);
        }
    };

    void print(std::ostream& out) const override
    {
        COMMANDS::SPI_WRITE::response_t response(
            (uint8_t*)&m_response.data()[4]);
        out << "SPI_WRITE  :";
    };

    COMMANDS::SPI_WRITE::response_t responseStruct()
    {
        return { (uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH] };
    };
};

