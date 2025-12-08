#pragma once
// This file is generated with the script: `interface/libs/commands/generate.py`

#include <common/raduinoCommandBase.hpp>

class RaduinoCommandGetAttachedDevicesCsvString : public RaduinoCommandBase {
public:
    RaduinoCommandGetAttachedDevicesCsvString()
        : RaduinoCommandBase(
              static_cast<uint8_t>(COMMANDS::OI::GET_ATTACHED_DEVICES_CSV_STRING),
              COMMANDS::GET_ATTACHED_DEVICES_CSV_STRING::COMMAND_LENGTH)
    {
        COMMANDS::GET_ATTACHED_DEVICES_CSV_STRING::command_t command;
    };

    void printResponse(std::ostream& out, COMMANDS::GET_ATTACHED_DEVICES_CSV_STRING::response_t response) const
    {
        out << "GET_ATTACHED_DEVICES_CSV_STRING   : ";
        out << " csvString=\"";
        for (uint8_t i = 0; i < 64; i++) {
            if (response.csvString[i])
            {
                out << static_cast<char>(response.csvString[i]);
            }
        }
        out << "\"";
    }

    std::string getCsvstring()
    {
        std::string retval;
        COMMANDS::GET_ATTACHED_DEVICES_CSV_STRING::response_t response = responseStruct();

        for (uint8_t i = 0; i < 64; i++) {
            if (response.csvString[i])
            {
                retval.push_back(static_cast<char>(response.csvString[i]));
            }
        }

        return (retval);
    }
    std::string getCommandName() { return "get_attached_devices_csv_string"; }

    std::string getJson()
    {
        std::string json;
        json.append("{");
        json.append("\"name\":\"" + getCommandName() + "\", ");
        json.append(getJsonCommonFields());
        json.append("\"payload\":{");
        json.append("\"csvString\": ");
        json.append("\"" + getCsvstring() + "\"");
        json.append("");
        json.append("}}");
        return (json);
    };

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::GET_ATTACHED_DEVICES_CSV_STRING::RESPONSE_LENGTH + 4)) {
            COMMANDS::GET_ATTACHED_DEVICES_CSV_STRING::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        }
        else {
            std::cout << "GET_ATTACHED_DEVICES_CSV_STRING: insufficient data" << std::endl;
        }
    };

    COMMANDS::GET_ATTACHED_DEVICES_CSV_STRING::response_t responseStruct()
    {
        COMMANDS::GET_ATTACHED_DEVICES_CSV_STRING::response_t response;

        if (m_responsePayload.size() >= sizeof(response)) {
            return { (uint8_t*)&m_responsePayload[0] };
        }

        return (response);
    };
};

