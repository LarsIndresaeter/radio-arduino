#pragma once

#include <common/uartCommandBase.hpp>

class UartCommandSha1 : public UartCommandBase {
public:
    UartCommandSha1(std::vector<uint8_t> data)
        : UartCommandBase(
            static_cast<uint8_t>(COMMANDS::OI::SHA1),
            COMMANDS::SHA1::COMMAND_LENGTH)
    {
        COMMANDS::SHA1::command_t command;

        for (int i = 0; i < data.size(); i++) {
            if (i >= sizeof(command.data)) {
                break;
            }
            m_payload.at(
                offsetof(COMMANDS::SHA1::command_t, data[0]) + i)
                = data.at(i);
        }

    };

    void printResponse(std::ostream& out, COMMANDS::SHA1::response_t response) const
    {
        out << "SHA1   : ";
        UartCommandBase::print(out);
        //for (int i = 0; i < sizeof(response.data); i++) {
            //out << " " + std::to_string(response.data[i]) + " ";
        //}
    }

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::SHA1::RESPONSE_LENGTH + 4)) {
            COMMANDS::SHA1::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        } else
        {
            std::cout << "SHA1: insufficient data" << std::endl;
        }
    };

    COMMANDS::SHA1::response_t responseStruct()
    {
        return { (uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH] };
    };
};

