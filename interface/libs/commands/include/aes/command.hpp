#pragma once
// This file is generated with the script: `interface/libs/commands/generate.py`

#include <common/uartCommandBase.hpp>

class UartCommandAes : public UartCommandBase {
public:
    UartCommandAes(uint8_t type, std::vector<uint8_t> data)
        : UartCommandBase(
            static_cast<uint8_t>(COMMANDS::OI::AES),
            COMMANDS::AES::COMMAND_LENGTH)
    {
        COMMANDS::AES::command_t command;

        m_payload.at(offsetof(COMMANDS::AES::command_t, type)) = type;

        for (int i = 0; i < data.size(); i++) {
            if (i >= data.size()) {
                break;
            }
            m_payload.at(
                offsetof(COMMANDS::AES::command_t, data[0]) + i)
                = data.at(i);
        }

    };

    void printResponse(std::ostream& out, COMMANDS::AES::response_t response) const
    {
        out << "AES                    : ";
        out << " type=" << static_cast<int>(response.getType());
        out << " data=[ ";
        out << std::setfill('0') << std::hex << std::uppercase;
        for(uint8_t i=0; i<16; i++)
        {
            out << std::setw(2) << static_cast<int>(response.data[i]) << " ";
        }
        out << "]";
        out << std::dec;
    }

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::AES::RESPONSE_LENGTH + 4)) {
            COMMANDS::AES::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        } else
        {
            std::cout << "AES: insufficient data" << std::endl;
        }
    };

    COMMANDS::AES::response_t responseStruct()
    {
        return { (uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH] };
    };
};

