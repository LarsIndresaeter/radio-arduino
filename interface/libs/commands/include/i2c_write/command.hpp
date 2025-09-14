#pragma once

#include <common/uartCommandBase.hpp>

class UartCommandI2cWrite : public UartCommandBase {
public:
    UartCommandI2cWrite(uint8_t device, uint16_t registerAddress, std::vector<uint8_t> data)
        : UartCommandBase(
            static_cast<uint8_t>(COMMANDS::OI::I2C_WRITE),
            COMMANDS::I2C_WRITE::COMMAND_LENGTH)
    {
        COMMANDS::I2C_WRITE::command_t command;

        m_payload.at(offsetof(COMMANDS::I2C_WRITE::command_t, device)) = device;
        m_payload.at(offsetof(COMMANDS::I2C_WRITE::command_t, registerHigh))
            = static_cast<uint16_t>(registerAddress) >> 8;
        m_payload.at(offsetof(COMMANDS::I2C_WRITE::command_t, registerLow))
            = registerAddress;

        if (data.size() < sizeof(command.data)) {
            m_payload.at(offsetof(COMMANDS::I2C_WRITE::command_t, length))
                = data.size();
        }
        else {
            m_payload.at(offsetof(COMMANDS::I2C_WRITE::command_t, length)) = sizeof(command.data);
        }

        for (int i = 0; i < data.size(); i++) {
            if (i >= sizeof(command.data)) {
                std::cout << "WARNING: I2C payload truncated after "
                          << static_cast<int>(
                                 sizeof(command.data))
                          << " bytes" << std::endl;
                break;
            }
            m_payload.at(offsetof(COMMANDS::I2C_WRITE::command_t, data[0]) + i)
                = data.at(i);

            // std::cout << "DEBUG: data[" << static_cast<int>(i) << "]=" <<
            // static_cast<int>(data.at(i)) << std::endl;
        }
    };

    void print(std::ostream& out) const override
    {
        COMMANDS::I2C_WRITE::response_t response(
            (uint8_t*)&m_response.data()[4]);
        out << "I2C_WRITE : status=" << static_cast<int>(response.status) << " ";

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
    };

    COMMANDS::I2C_WRITE::response_t responseStruct()
    {
        return { (uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH] };
    };
};

