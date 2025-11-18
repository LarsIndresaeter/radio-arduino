#pragma once
// This file is generated with the script: `interface/libs/commands/generate.py`

#include <common/raduinoCommandBase.hpp>

class RaduinoCommandGetUniqueId : public RaduinoCommandBase {
public:
    RaduinoCommandGetUniqueId()
        : RaduinoCommandBase(
              static_cast<uint8_t>(COMMANDS::OI::GET_UNIQUE_ID),
              COMMANDS::GET_UNIQUE_ID::COMMAND_LENGTH)
    {
        COMMANDS::GET_UNIQUE_ID::command_t command;
    };

    void printResponse(std::ostream& out, COMMANDS::GET_UNIQUE_ID::response_t response) const
    {
        out << "GET_UNIQUE_ID          : ";
        out << " id=" << static_cast<uint32_t>(response.getId());
    }

    std::string getCommandName() { return "get_unique_id"; }

    std::string getJson()
    {
        std::string json;
        json.append("{");
        json.append("\"name\":");
        json.append("\"get_unique_id\", ");
        json.append("\"timestamp\":");
        json.append(std::to_string(getTimeStamp()));
        json.append(", ");
        json.append("\"id\":");
        json.append(std::to_string(responseStruct().getId()));
        json.append("}");
        return (json);
    };

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::GET_UNIQUE_ID::RESPONSE_LENGTH + 4)) {
            COMMANDS::GET_UNIQUE_ID::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        }
        else {
            std::cout << "GET_UNIQUE_ID: insufficient data" << std::endl;
        }
    };

    COMMANDS::GET_UNIQUE_ID::response_t responseStruct()
    {
        COMMANDS::GET_UNIQUE_ID::response_t response;

        if (m_responsePayload.size() >= sizeof(response)) {
            return { (uint8_t*)&m_responsePayload[0] };
        }

        return (response);
    };
};

