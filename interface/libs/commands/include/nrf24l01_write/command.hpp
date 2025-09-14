#pragma once

#include <common/uartCommandBase.hpp>

class UartCommandNrf24l01Write : public UartCommandBase {
public:
    UartCommandNrf24l01Write(uint8_t length, std::vector<uint8_t> data)
        : UartCommandBase(
            static_cast<uint8_t>(COMMANDS::OI::NRF24L01_WRITE),
            COMMANDS::NRF24L01_WRITE::COMMAND_LENGTH)
    {
        COMMANDS::NRF24L01_WRITE::command_t command;

        m_payload.at(offsetof(COMMANDS::NRF24L01_WRITE::command_t, length)) = length;

        for (int i = 0; i < data.size(); i++) {
            if (i >= sizeof(command.data)) {
                break;
            }
            m_payload.at(
                offsetof(COMMANDS::NRF24L01_WRITE::command_t, data[0]) + i)
                = data.at(i);
        }

    };

    void printResponse(std::ostream& out, COMMANDS::NRF24L01_WRITE::response_t response) const
    {
        out << "NRF24L01_WRITE   : ";
        if(response.status == 1)
        {
            out << "OK";
        }
        else
        {
            out << "FAILED";
        }

        out << std::setfill('0') << std::setw(2) << std::hex << std::uppercase;

        for (int i = 0;
             (i < response.length) && (i < sizeof(response.data));
             i++) {
            out << " 0x" << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << static_cast<int>(response.data[i]);
        }
        out << " ]";
    }

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::NRF24L01_WRITE::RESPONSE_LENGTH + 4)) {
            COMMANDS::NRF24L01_WRITE::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        } else
        {
            std::cout << "NRF24L01_WRITE: insufficient data" << std::endl;
        }
    };

    COMMANDS::NRF24L01_WRITE::response_t responseStruct()
    {
        return { (uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH] };
    };
};

