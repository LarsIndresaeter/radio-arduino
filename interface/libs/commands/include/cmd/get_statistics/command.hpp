#pragma once
// This file is generated with the script: `interface/libs/commands/generate.py`

#include <common/uartCommandBase.hpp>

class UartCommandGetStatistics : public UartCommandBase {
public:
    UartCommandGetStatistics()
        : UartCommandBase(
              static_cast<uint8_t>(COMMANDS::OI::GET_STATISTICS),
              COMMANDS::GET_STATISTICS::COMMAND_LENGTH)
    {
        COMMANDS::GET_STATISTICS::command_t command;

    };

    void printResponse(std::ostream& out, COMMANDS::GET_STATISTICS::response_t response) const
    {
        out << "GET_STATISTICS         : ";
        out << " commandsParsed=" << static_cast<int>(response.getCommandsparsed());
        out << " uart_rx=" << static_cast<int>(response.getUart_rx());
        out << " uart_tx=" << static_cast<int>(response.getUart_tx());
        out << " rf_rx=" << static_cast<int>(response.getRf_rx());
        out << " rf_tx=" << static_cast<int>(response.getRf_tx());
    }


    std::string getCommandName() { return "get_statistics";}

    std::string getJson() {
        std::string json;
        json.append("{");
        json.append("\"name\":");
        json.append("\"get_statistics\", ");
        json.append("\"timestamp\":");
        json.append(std::to_string(getTimeStamp()));
        json.append(", ");
        json.append("\"commandsParsed\":");
        json.append(std::to_string(responseStruct().getCommandsparsed()));
        json.append(", ");
        json.append("\"uart_rx\":");
        json.append(std::to_string(responseStruct().getUart_rx()));
        json.append(", ");
        json.append("\"uart_tx\":");
        json.append(std::to_string(responseStruct().getUart_tx()));
        json.append(", ");
        json.append("\"rf_rx\":");
        json.append(std::to_string(responseStruct().getRf_rx()));
        json.append(", ");
        json.append("\"rf_tx\":");
        json.append(std::to_string(responseStruct().getRf_tx()));
        json.append("}");
        return(json);
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

