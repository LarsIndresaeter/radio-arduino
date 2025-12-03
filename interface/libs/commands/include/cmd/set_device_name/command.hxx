#pragma once
// This file is generated with the script: `interface/libs/commands/generate.py`

#include <common/raduinoCommandBase.hpp>

class RaduinoCommandSetDeviceName : public RaduinoCommandBase {
public:
    RaduinoCommandSetDeviceName(std::vector<uint8_t> name)
        : RaduinoCommandBase(
              static_cast<uint8_t>(COMMANDS::OI::SET_DEVICE_NAME),
              COMMANDS::SET_DEVICE_NAME::COMMAND_LENGTH)
    {
        COMMANDS::SET_DEVICE_NAME::command_t command;

        for (uint8_t i = 0; i < sizeof(command.name); i++) {
            if (i >= name.size()) {
                break;
            }
            m_payload.at(
                offsetof(COMMANDS::SET_DEVICE_NAME::command_t, name[0]) + i)
                = name.at(i);
        }
    };

    // string constructor
    RaduinoCommandSetDeviceName(std::string name)
        : RaduinoCommandBase(
              static_cast<uint8_t>(COMMANDS::OI::SET_DEVICE_NAME),
              COMMANDS::SET_DEVICE_NAME::COMMAND_LENGTH)
    {
        COMMANDS::SET_DEVICE_NAME::command_t command;

        for (uint8_t i = 0; i < name.size() && i < 16; i++) {
            m_payload.at(
                offsetof(COMMANDS::SET_DEVICE_NAME::command_t, name[0]) + i)
                = name.at(i);
        }
    };

    void printResponse(std::ostream& out, COMMANDS::SET_DEVICE_NAME::response_t response) const
    {
        out << "SET_DEVICE_NAME        : ";
    }

    std::string getCommandName() { return "set_device_name"; }

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
        if (m_response.size() >= (COMMANDS::SET_DEVICE_NAME::RESPONSE_LENGTH + 4)) {
            COMMANDS::SET_DEVICE_NAME::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        }
        else {
            std::cout << "SET_DEVICE_NAME: insufficient data" << std::endl;
        }
    };

    COMMANDS::SET_DEVICE_NAME::response_t responseStruct()
    {
        COMMANDS::SET_DEVICE_NAME::response_t response;

        if (m_responsePayload.size() >= sizeof(response)) {
            return { (uint8_t*)&m_responsePayload[0] };
        }

        return (response);
    };
};

