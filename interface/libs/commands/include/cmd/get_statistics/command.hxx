#pragma once
// This file is generated with the script: `interface/libs/commands/generate.py`

#include <common/raduinoCommandBase.hpp>

class RaduinoCommandGetStatistics : public RaduinoCommandBase {
public:
    RaduinoCommandGetStatistics()
        : RaduinoCommandBase(
              static_cast<uint8_t>(COMMANDS::OI::GET_STATISTICS),
              COMMANDS::GET_STATISTICS::COMMAND_LENGTH)
    {
        COMMANDS::GET_STATISTICS::command_t command;
    };

    void printResponse(std::ostream& out, COMMANDS::GET_STATISTICS::response_t response) const
    {
        out << "GET_STATISTICS         : ";
        out << " commandsParsed=" << static_cast<uint32_t>(response.getCommandsparsed());
        out << " uartRx=" << static_cast<uint32_t>(response.getUartrx());
        out << " uartTx=" << static_cast<uint32_t>(response.getUarttx());
        out << " radioRx=" << static_cast<uint32_t>(response.getRadiorx());
        out << " radioTx=" << static_cast<uint32_t>(response.getRadiotx());
        out << " restarts=" << static_cast<uint32_t>(response.getRestarts());
    }

    std::string getCommandName() { return "get_statistics"; }

    std::string getJson()
    {
        std::string json;
        json.append("{");
        json.append("\"name\":\"" + getCommandName() + "\", ");
        json.append(getJsonCommonFields());
        json.append("\"payload\":{");
        json.append("\"commandsParsed\":");
        json.append(std::to_string(responseStruct().getCommandsparsed()));
        json.append(", ");
        json.append("\"uartRx\":");
        json.append(std::to_string(responseStruct().getUartrx()));
        json.append(", ");
        json.append("\"uartTx\":");
        json.append(std::to_string(responseStruct().getUarttx()));
        json.append(", ");
        json.append("\"radioRx\":");
        json.append(std::to_string(responseStruct().getRadiorx()));
        json.append(", ");
        json.append("\"radioTx\":");
        json.append(std::to_string(responseStruct().getRadiotx()));
        json.append(", ");
        json.append("\"restarts\":");
        json.append(std::to_string(responseStruct().getRestarts()));
        json.append("}}");
        return (json);
    };

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::GET_STATISTICS::RESPONSE_LENGTH + 4)) {
            COMMANDS::GET_STATISTICS::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        }
        else {
            std::cout << "GET_STATISTICS: insufficient data" << std::endl;
        }
    };

    COMMANDS::GET_STATISTICS::response_t responseStruct()
    {
        COMMANDS::GET_STATISTICS::response_t response;

        if (m_responsePayload.size() >= sizeof(response)) {
            return { (uint8_t*)&m_responsePayload[0] };
        }

        return (response);
    };
};

