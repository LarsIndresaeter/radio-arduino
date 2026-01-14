#pragma once
// This file is generated with the script: `interface/libs/commands/generate.py`

#include <common/raduinoCommandBase.hpp>

class RaduinoCommandGetLastDeviceIdSeen : public RaduinoCommandBase {
public:
    RaduinoCommandGetLastDeviceIdSeen()
        : RaduinoCommandBase(
              static_cast<uint8_t>(COMMANDS::OI::GET_LAST_DEVICE_ID_SEEN),
              COMMANDS::GET_LAST_DEVICE_ID_SEEN::COMMAND_LENGTH)
    {
        COMMANDS::GET_LAST_DEVICE_ID_SEEN::command_t command;
    };

    void printResponse(std::ostream& out, COMMANDS::GET_LAST_DEVICE_ID_SEEN::response_t response) const
    {
        out << "GET_LAST_DEVICE_ID_SEEN   : ";
        out << " id=" << static_cast<uint32_t>(response.getId());
    }

    std::string getCommandName() { return "get_last_device_id_seen"; }

    std::string getJson()
    {
        std::string json;
        json.append("{");
        json.append("\"name\":\"" + getCommandName() + "\", ");
        json.append(getJsonCommonFields());
        json.append("\"payload\":{");
        json.append("\"id\":");
        json.append(std::to_string(responseStruct().getId()));
        json.append("}}");
        return (json);
    };

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::GET_LAST_DEVICE_ID_SEEN::RESPONSE_LENGTH + 4)) {
            COMMANDS::GET_LAST_DEVICE_ID_SEEN::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        }
        else {
            std::cout << "GET_LAST_DEVICE_ID_SEEN: insufficient data" << std::endl;
        }
    };

    COMMANDS::GET_LAST_DEVICE_ID_SEEN::response_t responseStruct()
    {
        COMMANDS::GET_LAST_DEVICE_ID_SEEN::response_t response;

        if (m_responsePayload.size() >= sizeof(response)) {
            return { (uint8_t*)&m_responsePayload[0] };
        }

        return (response);
    };
};

