#pragma once

#include <common/uartCommandBase.hpp>

class UartCommandI2cRead : public UartCommandBase {
public:
    UartCommandI2cRead(uint8_t device, uint16_t registerAddress, uint8_t length)
        : UartCommandBase(
            static_cast<uint8_t>(COMMANDS::OI::I2C_READ),
            COMMANDS::I2C_READ::COMMAND_LENGTH)
    {
        COMMANDS::I2C_READ::command_t command;

        m_payload.at(offsetof(COMMANDS::I2C_READ::command_t, device)) = device;

        m_payload.at(offsetof(COMMANDS::I2C_READ::command_t, registerAddress) + 1) = registerAddress>>8;
        m_payload.at(offsetof(COMMANDS::I2C_READ::command_t, registerAddress)) = registerAddress;

        m_payload.at(offsetof(COMMANDS::I2C_READ::command_t, length)) = length;

    };

    void printResponse(std::ostream& out, COMMANDS::I2C_READ::response_t response) const
    {
        out << "I2C_READ   : ";

        out << " device=" << static_cast<int>(response.device);
        out << " registerAddress=" << static_cast<int>(response.getRegisteraddress());
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

        uint16_t registerOffset = response.getRegisteraddress();
        for (int i = 0; (i < response.length) && (i < sizeof(response.data)); i++) {
            out << " data[" << static_cast<int>(i) << "] [" << static_cast<int>(registerOffset + i) << "]="
                << static_cast<int>(response.data[i]) << std::endl;
        }
    };

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::I2C_READ::RESPONSE_LENGTH + 4)) {
            COMMANDS::I2C_READ::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        } else
        {
            std::cout << "I2C_READ: insufficient data" << std::endl;
        }
    };

    COMMANDS::I2C_READ::response_t responseStruct()
    {
        return { (uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH] };
    };
};

