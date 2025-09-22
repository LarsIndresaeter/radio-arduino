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
        out << " bytesReceived=" << static_cast<int>(response.getBytesreceived());
        out << " bytesSent=" << static_cast<int>(response.getBytessent());
    }

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

