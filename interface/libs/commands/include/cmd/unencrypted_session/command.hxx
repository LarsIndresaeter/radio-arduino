#pragma once
// This file is generated with the script: `interface/libs/commands/generate.py`

#include <common/raduinoCommandBase.hpp>

class RaduinoCommandUnencryptedSession : public RaduinoCommandBase {
public:
    RaduinoCommandUnencryptedSession()
        : RaduinoCommandBase(
              static_cast<uint8_t>(COMMANDS::OI::UNENCRYPTED_SESSION),
              COMMANDS::UNENCRYPTED_SESSION::COMMAND_LENGTH)
    {
        COMMANDS::UNENCRYPTED_SESSION::command_t command;
    };

    void printResponse(std::ostream& out, COMMANDS::UNENCRYPTED_SESSION::response_t response) const
    {
        out << "UNENCRYPTED_SESSION    : ";
    }

    std::string getCommandName() { return "unencrypted_session"; }

    std::string getJson()
    {
        std::string json;
        json.append("{");
        json.append("\"name\":");
        json.append("\"unencrypted_session\", ");
        json.append("\"timestamp\":");
        json.append(std::to_string(getTimeStamp()));
        json.append(", ");
        json.append("}");
        return (json);
    };

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::UNENCRYPTED_SESSION::RESPONSE_LENGTH + 4)) {
            COMMANDS::UNENCRYPTED_SESSION::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        }
        else {
            std::cout << "UNENCRYPTED_SESSION: insufficient data" << std::endl;
        }
    };

    COMMANDS::UNENCRYPTED_SESSION::response_t responseStruct()
    {
        COMMANDS::UNENCRYPTED_SESSION::response_t response;

        if (m_responsePayload.size() >= sizeof(response)) {
            return { (uint8_t*)&m_responsePayload[0] };
        }

        return (response);
    };
};

