#pragma once
// This file is generated with the script: `interface/libs/commands/generate.py`

#include <common/raduinoCommandBase.hpp>

class RaduinoCommandSetAdvertisementInterval : public RaduinoCommandBase {
public:
    RaduinoCommandSetAdvertisementInterval(uint16_t interval)
        : RaduinoCommandBase(
              static_cast<uint8_t>(COMMANDS::OI::SET_ADVERTISEMENT_INTERVAL),
              COMMANDS::SET_ADVERTISEMENT_INTERVAL::COMMAND_LENGTH)
    {
        COMMANDS::SET_ADVERTISEMENT_INTERVAL::command_t command;

        m_payload.at(offsetof(COMMANDS::SET_ADVERTISEMENT_INTERVAL::command_t, interval) + 1) = interval>>8;
        m_payload.at(offsetof(COMMANDS::SET_ADVERTISEMENT_INTERVAL::command_t, interval)) = interval;
    };

    void printResponse(std::ostream& out, COMMANDS::SET_ADVERTISEMENT_INTERVAL::response_t response) const
    {
        out << "SET_ADVERTISEMENT_INTERVAL   : ";
    }

    std::string getCommandName() { return "set_advertisement_interval"; }

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
        if (m_response.size() >= (COMMANDS::SET_ADVERTISEMENT_INTERVAL::RESPONSE_LENGTH + 4)) {
            COMMANDS::SET_ADVERTISEMENT_INTERVAL::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        }
        else {
            std::cout << "SET_ADVERTISEMENT_INTERVAL: insufficient data" << std::endl;
        }
    };

    COMMANDS::SET_ADVERTISEMENT_INTERVAL::response_t responseStruct()
    {
        COMMANDS::SET_ADVERTISEMENT_INTERVAL::response_t response;

        if (m_responsePayload.size() >= sizeof(response)) {
            return { (uint8_t*)&m_responsePayload[0] };
        }

        return (response);
    };
};

