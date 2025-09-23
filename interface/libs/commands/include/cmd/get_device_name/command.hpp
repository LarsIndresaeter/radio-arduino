#pragma once
// This file is generated with the script: `interface/libs/commands/generate.py`

#include <common/uartCommandBase.hpp>

class UartCommandGetDeviceName : public UartCommandBase {
public:
    UartCommandGetDeviceName()
        : UartCommandBase(
              static_cast<uint8_t>(COMMANDS::OI::GET_DEVICE_NAME),
              COMMANDS::GET_DEVICE_NAME::COMMAND_LENGTH)
    {
        COMMANDS::GET_DEVICE_NAME::command_t command;

    };

    void printResponse(std::ostream& out, COMMANDS::GET_DEVICE_NAME::response_t response) const
    {
        out << "GET_DEVICE_NAME        : ";
        out << " nameString=\"";
        for (uint8_t i = 0; i < 16; i++) {
            if(response.nameString[i])
            {
                out << static_cast<char>(response.nameString[i]);
            }
        }
        out << "\"";
    }

    std::string getNamestring() {
        std::string retval;
        COMMANDS::GET_DEVICE_NAME::response_t response = responseStruct();

        retval.append("\"");
        for (uint8_t i = 0; i < 16; i++) {
            if(response.nameString[i])
            {
                retval.push_back(static_cast<char>(response.nameString[i]));
            }
        }
        retval.append("\"");

        return(retval);
    }

    std::string getCommandName() { return "get_device_name";}

    std::string getJson() {
        std::string json;
        json.append("{");
        json.append("\"name\":");
        json.append("\"get_device_name\", ");
        json.append("\"timestamp\":");
        json.append(std::to_string(getTimeStamp()));
        json.append(", ");
        json.append("\"nameString\": ");
        json.append(getNamestring());
        json.append("");
        json.append("}");
        return(json);
    };

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::GET_DEVICE_NAME::RESPONSE_LENGTH + 4)) {
            COMMANDS::GET_DEVICE_NAME::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        }
        else {
            std::cout << "GET_DEVICE_NAME: insufficient data" << std::endl;
        }
    };

    COMMANDS::GET_DEVICE_NAME::response_t responseStruct()
    {
        COMMANDS::GET_DEVICE_NAME::response_t response;

        if (m_responsePayload.size() >= sizeof(response)) {
            return { (uint8_t*)&m_responsePayload[0] };
        }

        return (response);
    };
};

