#pragma once
// This file is generated with the script: `interface/libs/commands/generate.py`

#include <common/raduinoCommandBase.hpp>

class RaduinoCommandSpiWrite : public RaduinoCommandBase {
public:
    RaduinoCommandSpiWrite(uint8_t reg, uint8_t length, std::vector<uint8_t> data)
        : RaduinoCommandBase(
              static_cast<uint8_t>(COMMANDS::OI::SPI_WRITE),
              COMMANDS::SPI_WRITE::COMMAND_LENGTH)
    {
        COMMANDS::SPI_WRITE::command_t command;

        m_payload.at(offsetof(COMMANDS::SPI_WRITE::command_t, reg)) = reg;

        m_payload.at(offsetof(COMMANDS::SPI_WRITE::command_t, length)) = length;

        for (int i = 0; i < sizeof(command.data); i++) {
            if (i >= data.size()) {
                break;
            }
            m_payload.at(
                offsetof(COMMANDS::SPI_WRITE::command_t, data[0]) + i)
                = data.at(i);
        }
    };

    // string constructor
    RaduinoCommandSpiWrite(uint8_t reg, uint8_t length, std::string data)
        : RaduinoCommandBase(
              static_cast<uint8_t>(COMMANDS::OI::SPI_WRITE),
              COMMANDS::SPI_WRITE::COMMAND_LENGTH)
    {
        COMMANDS::SPI_WRITE::command_t command;

        m_payload.at(offsetof(COMMANDS::SPI_WRITE::command_t, reg)) = reg;

        m_payload.at(offsetof(COMMANDS::SPI_WRITE::command_t, length)) = length;

        for (int i = 0; i < data.size() && i < 32; i++) {
            m_payload.at(
                offsetof(COMMANDS::SPI_WRITE::command_t, data[0]) + i)
                = data.at(i);
        }
    };

    void printResponse(std::ostream& out, COMMANDS::SPI_WRITE::response_t response) const
    {
        out << "SPI_WRITE              : ";
    }

    std::string getCommandName() { return "spi_write"; }

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
        if (m_response.size() >= (COMMANDS::SPI_WRITE::RESPONSE_LENGTH + 4)) {
            COMMANDS::SPI_WRITE::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        }
        else {
            std::cout << "SPI_WRITE: insufficient data" << std::endl;
        }
    };

    COMMANDS::SPI_WRITE::response_t responseStruct()
    {
        COMMANDS::SPI_WRITE::response_t response;

        if (m_responsePayload.size() >= sizeof(response)) {
            return { (uint8_t*)&m_responsePayload[0] };
        }

        return (response);
    };
};

