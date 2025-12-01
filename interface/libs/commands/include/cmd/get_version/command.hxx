#pragma once
// This file is generated with the script: `interface/libs/commands/generate.py`

#include <common/raduinoCommandBase.hpp>

class RaduinoCommandGetVersion : public RaduinoCommandBase {
public:
    RaduinoCommandGetVersion()
        : RaduinoCommandBase(
              static_cast<uint8_t>(COMMANDS::OI::GET_VERSION),
              COMMANDS::GET_VERSION::COMMAND_LENGTH)
    {
        COMMANDS::GET_VERSION::command_t command;
    };

    void printResponse(std::ostream& out, COMMANDS::GET_VERSION::response_t response) const
    {
        out << "GET_VERSION            : ";
        out << " versionString=\"";
        for (uint8_t i = 0; i < 32; i++) {
            if (response.versionString[i])
            {
                out << static_cast<char>(response.versionString[i]);
            }
        }
        out << "\"";
    }

    std::string getVersionstring()
    {
        std::string retval;
        COMMANDS::GET_VERSION::response_t response = responseStruct();

        for (uint8_t i = 0; i < 32; i++) {
            if (response.versionString[i])
            {
                retval.push_back(static_cast<char>(response.versionString[i]));
            }
        }

        return (retval);
    }
    std::string getCommandName() { return "get_version"; }

    std::string getJson()
    {
        std::string json;
        json.append("{");
        json.append("\"name\":\"" + getCommandName() + "\", ");
        json.append(getJsonCommonFields());
        json.append("\"payload\":{");
        json.append("\"versionString\": ");
        json.append("\"" + getVersionstring() + "\"");
        json.append("");
        json.append("}}");
        return (json);
    };

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::GET_VERSION::RESPONSE_LENGTH + 4)) {
            COMMANDS::GET_VERSION::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        }
        else {
            std::cout << "GET_VERSION: insufficient data" << std::endl;
        }
    };

    COMMANDS::GET_VERSION::response_t responseStruct()
    {
        COMMANDS::GET_VERSION::response_t response;

        if (m_responsePayload.size() >= sizeof(response)) {
            return { (uint8_t*)&m_responsePayload[0] };
        }

        return (response);
    };
};

