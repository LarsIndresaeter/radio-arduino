#pragma once
// This file is generated with the script: `interface/libs/commands/generate.py`

#include <common/raduinoCommandBase.hpp>

class RaduinoCommandEepromRead : public RaduinoCommandBase {
public:
    RaduinoCommandEepromRead(uint16_t address)
        : RaduinoCommandBase(
              static_cast<uint8_t>(COMMANDS::OI::EEPROM_READ),
              COMMANDS::EEPROM_READ::COMMAND_LENGTH)
    {
        COMMANDS::EEPROM_READ::command_t command;

        m_payload.at(offsetof(COMMANDS::EEPROM_READ::command_t, address) + 1) = address>>8;
        m_payload.at(offsetof(COMMANDS::EEPROM_READ::command_t, address)) = address;
    };

    void printResponse(std::ostream& out, COMMANDS::EEPROM_READ::response_t response) const
    {
        out << "EEPROM_READ            : ";
        out << " address=" << static_cast<uint32_t>(response.getAddress());
        out << " data=" << static_cast<uint32_t>(response.getData());
    }

    std::string getCommandName() { return "eeprom_read"; }

    std::string getJson()
    {
        std::string json;
        json.append("{");
        json.append("\"name\":");
        json.append("\"eeprom_read\", ");
        json.append("\"timestamp\":");
        json.append(std::to_string(getTimeStamp()));
        json.append(", ");
        json.append("\"address\":");
        json.append(std::to_string(responseStruct().getAddress()));
        json.append(", ");
        json.append("\"data\":");
        json.append(std::to_string(responseStruct().getData()));
        json.append("}");
        return (json);
    };

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::EEPROM_READ::RESPONSE_LENGTH + 4)) {
            COMMANDS::EEPROM_READ::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        }
        else {
            std::cout << "EEPROM_READ: insufficient data" << std::endl;
        }
    };

    COMMANDS::EEPROM_READ::response_t responseStruct()
    {
        COMMANDS::EEPROM_READ::response_t response;

        if (m_responsePayload.size() >= sizeof(response)) {
            return { (uint8_t*)&m_responsePayload[0] };
        }

        return (response);
    };
};

