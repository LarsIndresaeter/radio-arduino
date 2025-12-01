#pragma once
// This file is generated with the script: `interface/libs/commands/generate.py`

#include <common/raduinoCommandBase.hpp>

class RaduinoCommandSetNodeAddress : public RaduinoCommandBase {
public:
    RaduinoCommandSetNodeAddress(uint8_t nodeAddress)
        : RaduinoCommandBase(
              static_cast<uint8_t>(COMMANDS::OI::SET_NODE_ADDRESS),
              COMMANDS::SET_NODE_ADDRESS::COMMAND_LENGTH)
    {
        COMMANDS::SET_NODE_ADDRESS::command_t command;

        m_payload.at(offsetof(COMMANDS::SET_NODE_ADDRESS::command_t, nodeAddress)) = nodeAddress;
    };

    void printResponse(std::ostream& out, COMMANDS::SET_NODE_ADDRESS::response_t response) const
    {
        out << "SET_NODE_ADDRESS       : ";
    }

    std::string getCommandName() { return "set_node_address"; }

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
        if (m_response.size() >= (COMMANDS::SET_NODE_ADDRESS::RESPONSE_LENGTH + 4)) {
            COMMANDS::SET_NODE_ADDRESS::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        }
        else {
            std::cout << "SET_NODE_ADDRESS: insufficient data" << std::endl;
        }
    };

    COMMANDS::SET_NODE_ADDRESS::response_t responseStruct()
    {
        COMMANDS::SET_NODE_ADDRESS::response_t response;

        if (m_responsePayload.size() >= sizeof(response)) {
            return { (uint8_t*)&m_responsePayload[0] };
        }

        return (response);
    };
};

