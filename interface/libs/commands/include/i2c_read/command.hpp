#pragma once

#include <common/uartCommandBase.hpp>

class UartCommandI2cRead : public UartCommandBase {
public:
    UartCommandI2cRead(uint8_t deviceAddress, uint16_t registerAddress, uint8_t length)

        : UartCommandBase(
            static_cast<uint8_t>(COMMANDS::OI::I2C_READ),
            COMMANDS::I2C_READ::COMMAND_LENGTH)
    {
        m_payload.at(offsetof(COMMANDS::I2C_READ::command_t, device))
            = deviceAddress;
        m_payload.at(offsetof(COMMANDS::I2C_READ::command_t, registerHigh))
            = static_cast<uint16_t>(registerAddress) >> 8;
        m_payload.at(offsetof(COMMANDS::I2C_READ::command_t, registerLow))
            = registerAddress;
        m_payload.at(offsetof(COMMANDS::I2C_READ::command_t, length))
            = length;
    };

    void print(std::ostream& out) const override
    {
        COMMANDS::I2C_READ::response_t response(
            (uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH]);

        out << "I2C_READ   : ";
        out << " device=" << static_cast<int>(response.device);
        out << " registerHigh=" << static_cast<int>(response.registerHigh);
        out << " registerLow=" << static_cast<int>(response.registerLow);
        out << " length=" << static_cast<int>(response.length);
        out << " status=" << static_cast<int>(response.status) << " ";

        if (0 == response.status)
        {
            out << "OK";
        }
        else if (1 == response.status)
        {
            out << "nack";
        }
        else if (2 == response.status)
        {
            out << "transmission failure";
        }

        out << std::endl;

        uint16_t registerOffset = (response.registerHigh<<8) + response.registerLow;
        for (int i = 0; (i < response.length) && (i < COMMANDS::I2C_READ::MAX_DATA_LENGTH); i++) {
            out << " data[" << static_cast<int>(i) << "] [" << static_cast<int>(registerOffset + i) << "]="
                << static_cast<int>(response.data[i]) << std::endl;
        }
    };

    COMMANDS::I2C_READ::response_t responseStruct()

    {
        return {(uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH]};
    };
};

