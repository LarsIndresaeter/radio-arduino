#pragma once
// This file is generated with the script: `interface/libs/commands/generate.py`

#include <common/raduinoCommandBase.hpp>

class RaduinoCommandSetAttachedDevicesCsvString : public RaduinoCommandBase {
public:
    RaduinoCommandSetAttachedDevicesCsvString(std::vector<uint8_t> csvString)
        : RaduinoCommandBase(
              static_cast<uint8_t>(COMMANDS::OI::SET_ATTACHED_DEVICES_CSV_STRING),
              COMMANDS::SET_ATTACHED_DEVICES_CSV_STRING::COMMAND_LENGTH)
    {
        COMMANDS::SET_ATTACHED_DEVICES_CSV_STRING::command_t command;

        for (uint8_t i = 0; i < sizeof(command.csvString); i++) {
            if (i >= csvString.size()) {
                break;
            }
            m_payload.at(
                offsetof(COMMANDS::SET_ATTACHED_DEVICES_CSV_STRING::command_t, csvString[0]) + i)
                = csvString.at(i);
        }
    };

    // string constructor
    RaduinoCommandSetAttachedDevicesCsvString(std::string csvString)
        : RaduinoCommandBase(
              static_cast<uint8_t>(COMMANDS::OI::SET_ATTACHED_DEVICES_CSV_STRING),
              COMMANDS::SET_ATTACHED_DEVICES_CSV_STRING::COMMAND_LENGTH)
    {
        COMMANDS::SET_ATTACHED_DEVICES_CSV_STRING::command_t command;

        for (uint8_t i = 0; i < csvString.size() & i < 64; i++) {
            m_payload.at(
                offsetof(COMMANDS::SET_ATTACHED_DEVICES_CSV_STRING::command_t, csvString[0]) + i)
                = csvString.at(i);
        }
    };

    void printResponse(std::ostream& out, COMMANDS::SET_ATTACHED_DEVICES_CSV_STRING::response_t response) const
    {
        out << "SET_ATTACHED_DEVICES_CSV_STRING   : ";
    }

    std::string getCommandName() { return "set_attached_devices_csv_string"; }

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
        if (m_response.size() >= (COMMANDS::SET_ATTACHED_DEVICES_CSV_STRING::RESPONSE_LENGTH + 4)) {
            COMMANDS::SET_ATTACHED_DEVICES_CSV_STRING::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        }
        else {
            std::cout << "SET_ATTACHED_DEVICES_CSV_STRING: insufficient data" << std::endl;
        }
    };

    COMMANDS::SET_ATTACHED_DEVICES_CSV_STRING::response_t responseStruct()
    {
        COMMANDS::SET_ATTACHED_DEVICES_CSV_STRING::response_t response;

        if (m_responsePayload.size() >= sizeof(response)) {
            return { (uint8_t*)&m_responsePayload[0] };
        }

        return (response);
    };
};

