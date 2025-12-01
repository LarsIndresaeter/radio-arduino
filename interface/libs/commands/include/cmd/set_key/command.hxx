#pragma once
// This file is generated with the script: `interface/libs/commands/generate.py`

#include <common/raduinoCommandBase.hpp>

class RaduinoCommandSetKey : public RaduinoCommandBase {
public:
    RaduinoCommandSetKey(uint8_t keyId, std::vector<uint8_t> keyValue)
        : RaduinoCommandBase(
              static_cast<uint8_t>(COMMANDS::OI::SET_KEY),
              COMMANDS::SET_KEY::COMMAND_LENGTH)
    {
        COMMANDS::SET_KEY::command_t command;

        m_payload.at(offsetof(COMMANDS::SET_KEY::command_t, keyId)) = keyId;

        for (int i = 0; i < sizeof(command.keyValue); i++) {
            if (i >= keyValue.size()) {
                break;
            }
            m_payload.at(
                offsetof(COMMANDS::SET_KEY::command_t, keyValue[0]) + i)
                = keyValue.at(i);
        }
    };

    // string constructor
    RaduinoCommandSetKey(uint8_t keyId, std::string keyValue)
        : RaduinoCommandBase(
              static_cast<uint8_t>(COMMANDS::OI::SET_KEY),
              COMMANDS::SET_KEY::COMMAND_LENGTH)
    {
        COMMANDS::SET_KEY::command_t command;

        m_payload.at(offsetof(COMMANDS::SET_KEY::command_t, keyId)) = keyId;

        for (int i = 0; i < keyValue.size() && i < 16; i++) {
            m_payload.at(
                offsetof(COMMANDS::SET_KEY::command_t, keyValue[0]) + i)
                = keyValue.at(i);
        }
    };

    void printResponse(std::ostream& out, COMMANDS::SET_KEY::response_t response) const
    {
        out << "SET_KEY                : ";
    }

    std::string getCommandName() { return "set_key"; }

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
        if (m_response.size() >= (COMMANDS::SET_KEY::RESPONSE_LENGTH + 4)) {
            COMMANDS::SET_KEY::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        }
        else {
            std::cout << "SET_KEY: insufficient data" << std::endl;
        }
    };

    COMMANDS::SET_KEY::response_t responseStruct()
    {
        COMMANDS::SET_KEY::response_t response;

        if (m_responsePayload.size() >= sizeof(response)) {
            return { (uint8_t*)&m_responsePayload[0] };
        }

        return (response);
    };
};

