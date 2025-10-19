#pragma once
// This file is generated with the script: `interface/libs/commands/generate.py`

#include <common/raduinoCommandBase.hpp>

class RaduinoCommandUnlockSession : public RaduinoCommandBase {
public:
    RaduinoCommandUnlockSession()
        : RaduinoCommandBase(
              static_cast<uint8_t>(COMMANDS::OI::UNLOCK_SESSION),
              COMMANDS::UNLOCK_SESSION::COMMAND_LENGTH)
    {
        COMMANDS::UNLOCK_SESSION::command_t command;
    };

    void printResponse(std::ostream& out, COMMANDS::UNLOCK_SESSION::response_t response) const
    {
        out << "UNLOCK_SESSION         : ";
    }

    std::string getCommandName() { return "unlock_session"; }

    std::string getJson()
    {
        std::string json;
        json.append("{");
        json.append("\"name\":");
        json.append("\"unlock_session\", ");
        json.append("\"timestamp\":");
        json.append(std::to_string(getTimeStamp()));
        json.append(", ");
        json.append("}");
        return (json);
    };

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::UNLOCK_SESSION::RESPONSE_LENGTH + 4)) {
            COMMANDS::UNLOCK_SESSION::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        }
        else {
            std::cout << "UNLOCK_SESSION: insufficient data" << std::endl;
        }
    };

    COMMANDS::UNLOCK_SESSION::response_t responseStruct()
    {
        COMMANDS::UNLOCK_SESSION::response_t response;

        if (m_responsePayload.size() >= sizeof(response)) {
            return { (uint8_t*)&m_responsePayload[0] };
        }

        return (response);
    };
};

