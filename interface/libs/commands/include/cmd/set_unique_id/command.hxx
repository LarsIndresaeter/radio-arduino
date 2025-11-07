#pragma once
// This file is generated with the script: `interface/libs/commands/generate.py`

#include <common/raduinoCommandBase.hpp>

class RaduinoCommandSetUniqueId : public RaduinoCommandBase {
public:
    RaduinoCommandSetUniqueId(uint32_t id)
        : RaduinoCommandBase(
              static_cast<uint8_t>(COMMANDS::OI::SET_UNIQUE_ID),
              COMMANDS::SET_UNIQUE_ID::COMMAND_LENGTH)
    {
        COMMANDS::SET_UNIQUE_ID::command_t command;

        m_payload.at(offsetof(COMMANDS::SET_UNIQUE_ID::command_t, id) + 3) = id>>24;
        m_payload.at(offsetof(COMMANDS::SET_UNIQUE_ID::command_t, id) + 2) = id>>16;
        m_payload.at(offsetof(COMMANDS::SET_UNIQUE_ID::command_t, id) + 1) = id>>8;
        m_payload.at(offsetof(COMMANDS::SET_UNIQUE_ID::command_t, id)) = id;
    };

    void printResponse(std::ostream& out, COMMANDS::SET_UNIQUE_ID::response_t response) const
    {
        out << "SET_UNIQUE_ID          : ";
    }

    std::string getCommandName() { return "set_unique_id"; }

    std::string getJson()
    {
        std::string json;
        json.append("{");
        json.append("\"name\":");
        json.append("\"set_unique_id\", ");
        json.append("\"timestamp\":");
        json.append(std::to_string(getTimeStamp()));
        json.append(", ");
        json.append("}");
        return (json);
    };

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::SET_UNIQUE_ID::RESPONSE_LENGTH + 4)) {
            COMMANDS::SET_UNIQUE_ID::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        }
        else {
            std::cout << "SET_UNIQUE_ID: insufficient data" << std::endl;
        }
    };

    COMMANDS::SET_UNIQUE_ID::response_t responseStruct()
    {
        COMMANDS::SET_UNIQUE_ID::response_t response;

        if (m_responsePayload.size() >= sizeof(response)) {
            return { (uint8_t*)&m_responsePayload[0] };
        }

        return (response);
    };
};

