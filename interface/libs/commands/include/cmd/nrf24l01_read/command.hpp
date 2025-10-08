#pragma once
// This file is generated with the script: `interface/libs/commands/generate.py`

#include <common/uartCommandBase.hpp>

class UartCommandNrf24l01Read : public UartCommandBase {
public:
    UartCommandNrf24l01Read(uint8_t length, std::vector<uint8_t> data)
        : UartCommandBase(
              static_cast<uint8_t>(COMMANDS::OI::NRF24L01_READ),
              COMMANDS::NRF24L01_READ::COMMAND_LENGTH)
    {
        COMMANDS::NRF24L01_READ::command_t command;

        m_payload.at(offsetof(COMMANDS::NRF24L01_READ::command_t, length)) = length;

        for (int i = 0; i < sizeof(command.data); i++) {
            if (i >= data.size()) {
                break;
            }
            m_payload.at(
                offsetof(COMMANDS::NRF24L01_READ::command_t, data[0]) + i)
                = data.at(i);
        }
    };

    // string constructor
    UartCommandNrf24l01Read(uint8_t length, std::string data)
        : UartCommandBase(
              static_cast<uint8_t>(COMMANDS::OI::NRF24L01_READ),
              COMMANDS::NRF24L01_READ::COMMAND_LENGTH)
    {
        COMMANDS::NRF24L01_READ::command_t command;

        m_payload.at(offsetof(COMMANDS::NRF24L01_READ::command_t, length)) = length;

        for (int i = 0; i < data.size() && i < 128; i++) {
            m_payload.at(
                offsetof(COMMANDS::NRF24L01_READ::command_t, data[0]) + i)
                = data.at(i);
        }
    };

    void printResponse(std::ostream& out, COMMANDS::NRF24L01_READ::response_t response) const
    {
        out << "NRF24L01_READ          : ";
        out << " length=" << static_cast<int>(response.getLength());
        out << " data=[ ";
        out << std::setfill('0') << std::hex << std::uppercase;
        for (uint8_t i = 0; i < 128; i++) {
            out << std::setw(2) << static_cast<int>(response.data[i]) << " ";
        }
        out << "]";
        out << std::dec;
    }

    std::string getData()
    {
        std::string retval;
        COMMANDS::NRF24L01_READ::response_t response = responseStruct();

        retval.append("[");
        for (uint8_t i = 0; i < 128; i++) {
            retval.append(" \"");
            retval.append(std::to_string(static_cast<int>(response.data[i])));
            if (i < (128 - 1)) {
                retval.append("\",");
            }
            else {
                retval.append("\"");
            }
        }
        retval.append(" ]");

        return (retval);
    }
    std::string getCommandName() { return "nrf24l01_read"; }

    std::string getJson()
    {
        std::string json;
        json.append("{");
        json.append("\"name\":");
        json.append("\"nrf24l01_read\", ");
        json.append("\"timestamp\":");
        json.append(std::to_string(getTimeStamp()));
        json.append(", ");
        json.append("\"length\":");
        json.append(std::to_string(responseStruct().getLength()));
        json.append(", ");
        json.append("\"data\": ");
        json.append(getData());
        json.append("");
        json.append("}");
        return (json);
    };

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::NRF24L01_READ::RESPONSE_LENGTH + 4)) {
            COMMANDS::NRF24L01_READ::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        }
        else {
            std::cout << "NRF24L01_READ: insufficient data" << std::endl;
        }
    };

    COMMANDS::NRF24L01_READ::response_t responseStruct()
    {
        COMMANDS::NRF24L01_READ::response_t response;

        if (m_responsePayload.size() >= sizeof(response)) {
            return { (uint8_t*)&m_responsePayload[0] };
        }

        return (response);
    };
};

