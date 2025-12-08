#pragma once
// This file is generated with the script: `interface/libs/commands/generate.py`

#include <common/raduinoCommandBase.hpp>

class RaduinoCommandSha1 : public RaduinoCommandBase {
public:
    RaduinoCommandSha1(std::vector<uint8_t> data)
        : RaduinoCommandBase(
              static_cast<uint8_t>(COMMANDS::OI::SHA1),
              COMMANDS::SHA1::COMMAND_LENGTH)
    {
        COMMANDS::SHA1::command_t command;

        for (uint8_t i = 0; i < sizeof(command.data); i++) {
            if (i >= data.size()) {
                break;
            }
            m_payload.at(
                offsetof(COMMANDS::SHA1::command_t, data[0]) + i)
                = data.at(i);
        }
    };

    // string constructor
    RaduinoCommandSha1(std::string data)
        : RaduinoCommandBase(
              static_cast<uint8_t>(COMMANDS::OI::SHA1),
              COMMANDS::SHA1::COMMAND_LENGTH)
    {
        COMMANDS::SHA1::command_t command;

        for (uint8_t i = 0; i < data.size() & i < 20; i++) {
            m_payload.at(
                offsetof(COMMANDS::SHA1::command_t, data[0]) + i)
                = data.at(i);
        }
    };

    void printResponse(std::ostream& out, COMMANDS::SHA1::response_t response) const
    {
        out << "SHA1                   : ";
        out << " data=[ ";
        out << std::setfill('0') << std::hex << std::uppercase;
        for (uint8_t i = 0; i < 20; i++) {
            out << std::setw(2) << static_cast<uint32_t>(response.data[i]) << " ";
        }
        out << "]";
        out << std::dec;
    }

    std::string getData()
    {
        std::string retval;
        COMMANDS::SHA1::response_t response = responseStruct();

        retval.append("[");
        for (uint8_t i = 0; i < 20; i++) {
            retval.append(" \"");
            retval.append(std::to_string(static_cast<uint32_t>(response.data[i])));
            if (i < (20 - 1)) {
                retval.append("\",");
            }
            else {
                retval.append("\"");
            }
        }
        retval.append(" ]");

        return (retval);
    }
    std::string getCommandName() { return "sha1"; }

    std::string getJson()
    {
        std::string json;
        json.append("{");
        json.append("\"name\":\"" + getCommandName() + "\", ");
        json.append(getJsonCommonFields());
        json.append("\"payload\":{");
        json.append("\"data\": ");
        json.append("\"" + getData() + "\"");
        json.append("");
        json.append("}}");
        return (json);
    };

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::SHA1::RESPONSE_LENGTH + 4)) {
            COMMANDS::SHA1::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        }
        else {
            std::cout << "SHA1: insufficient data" << std::endl;
        }
    };

    COMMANDS::SHA1::response_t responseStruct()
    {
        COMMANDS::SHA1::response_t response;

        if (m_responsePayload.size() >= sizeof(response)) {
            return { (uint8_t*)&m_responsePayload[0] };
        }

        return (response);
    };
};

