#pragma once
// This file is generated with the script: `interface/libs/commands/generate.py`

#include <common/raduinoCommandBase.hpp>

class RaduinoCommandSetRadioChannel : public RaduinoCommandBase {
public:
    RaduinoCommandSetRadioChannel(uint8_t channel)
        : RaduinoCommandBase(
              static_cast<uint8_t>(COMMANDS::OI::SET_RADIO_CHANNEL),
              COMMANDS::SET_RADIO_CHANNEL::COMMAND_LENGTH)
    {
        COMMANDS::SET_RADIO_CHANNEL::command_t command;

        m_payload.at(offsetof(COMMANDS::SET_RADIO_CHANNEL::command_t, channel)) = channel;
    };

    void printResponse(std::ostream& out, COMMANDS::SET_RADIO_CHANNEL::response_t response) const
    {
        out << "SET_RADIO_CHANNEL      : ";
    }

    std::string getCommandName() { return "set_radio_channel"; }

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
        if (m_response.size() >= (COMMANDS::SET_RADIO_CHANNEL::RESPONSE_LENGTH + 4)) {
            COMMANDS::SET_RADIO_CHANNEL::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        }
        else {
            std::cout << "SET_RADIO_CHANNEL: insufficient data" << std::endl;
        }
    };

    COMMANDS::SET_RADIO_CHANNEL::response_t responseStruct()
    {
        COMMANDS::SET_RADIO_CHANNEL::response_t response;

        if (m_responsePayload.size() >= sizeof(response)) {
            return { (uint8_t*)&m_responsePayload[0] };
        }

        return (response);
    };
};

