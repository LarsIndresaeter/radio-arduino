#pragma once
// This file is generated with the script: `interface/libs/commands/generate.py`

#include <common/raduinoCommandBase.hpp>

class RaduinoCommandI2cWrite : public RaduinoCommandBase {
public:
    RaduinoCommandI2cWrite(uint8_t device, uint16_t registerAddress, uint8_t length, std::vector<uint8_t> data)
        : RaduinoCommandBase(
              static_cast<uint8_t>(COMMANDS::OI::I2C_WRITE),
              COMMANDS::I2C_WRITE::COMMAND_LENGTH)
    {
        COMMANDS::I2C_WRITE::command_t command;

        m_payload.at(offsetof(COMMANDS::I2C_WRITE::command_t, device)) = device;

        m_payload.at(offsetof(COMMANDS::I2C_WRITE::command_t, registerAddress) + 1) = registerAddress>>8;
        m_payload.at(offsetof(COMMANDS::I2C_WRITE::command_t, registerAddress)) = registerAddress;

        m_payload.at(offsetof(COMMANDS::I2C_WRITE::command_t, length)) = length;

        for (uint8_t i = 0; i < sizeof(command.data); i++) {
            if (i >= data.size()) {
                break;
            }
            m_payload.at(
                offsetof(COMMANDS::I2C_WRITE::command_t, data[0]) + i)
                = data.at(i);
        }
    };

    // string constructor
    RaduinoCommandI2cWrite(uint8_t device, uint16_t registerAddress, uint8_t length, std::string data)
        : RaduinoCommandBase(
              static_cast<uint8_t>(COMMANDS::OI::I2C_WRITE),
              COMMANDS::I2C_WRITE::COMMAND_LENGTH)
    {
        COMMANDS::I2C_WRITE::command_t command;

        m_payload.at(offsetof(COMMANDS::I2C_WRITE::command_t, device)) = device;

        m_payload.at(offsetof(COMMANDS::I2C_WRITE::command_t, registerAddress) + 1) = registerAddress>>8;
        m_payload.at(offsetof(COMMANDS::I2C_WRITE::command_t, registerAddress)) = registerAddress;

        m_payload.at(offsetof(COMMANDS::I2C_WRITE::command_t, length)) = length;

        for (uint8_t i = 0; i < data.size() && i < 16; i++) {
            m_payload.at(
                offsetof(COMMANDS::I2C_WRITE::command_t, data[0]) + i)
                = data.at(i);
        }
    };

    void printResponse(std::ostream& out, COMMANDS::I2C_WRITE::response_t response) const
    {
        out << "I2C_WRITE              : ";
    }

    std::string getCommandName() { return "i2c_write"; }

    std::string getJson()
    {
        std::string json;
        json.append("{");
        json.append("\"name\":\"" + getCommandName() + "\", ");
        json.append(getJsonCommonFields());
        json.append("\"payload\":{");
        json.append("}}");
        return (json);
    };

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::I2C_WRITE::RESPONSE_LENGTH + 4)) {
            COMMANDS::I2C_WRITE::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        }
        else {
            std::cout << "I2C_WRITE: insufficient data" << std::endl;
        }
    };

    COMMANDS::I2C_WRITE::response_t responseStruct()
    {
        COMMANDS::I2C_WRITE::response_t response;

        if (m_responsePayload.size() >= sizeof(response)) {
            return { (uint8_t*)&m_responsePayload[0] };
        }

        return (response);
    };
};

