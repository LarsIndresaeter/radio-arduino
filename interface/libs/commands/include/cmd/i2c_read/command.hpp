#pragma once
// This file is generated with the script: `interface/libs/commands/generate.py`

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
        out << "I2C_READ               : ";
        out << " device=" << static_cast<int>(response.getDevice());
        out << " registerAddress=" << static_cast<int>(response.getRegisteraddress());
        out << " length=" << static_cast<int>(response.getLength());
        out << " data=[ ";
        out << std::setfill('0') << std::hex << std::uppercase;
        for (uint8_t i = 0; i < 16; i++) {
            out << std::setw(2) << static_cast<int>(response.data[i]) << " ";
        }
        out << "]";
        out << std::dec;
    }

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::I2C_READ::RESPONSE_LENGTH + 4)) {
            COMMANDS::I2C_READ::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        }
        else {
            std::cout << "I2C_READ: insufficient data" << std::endl;
        }
    };

    COMMANDS::I2C_READ::response_t responseStruct()
    {
        COMMANDS::I2C_READ::response_t response;

        if (m_responsePayload.size() >= sizeof(response)) {
            return { (uint8_t*)&m_responsePayload[0] };
        }

        return (response);
    };
};

