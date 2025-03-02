#pragma once

#include <common/uartCommandBase.hpp>

class UartCommandSha1 : public UartCommandBase {
public:
    UartCommandSha1(std::vector<uint8_t> data)
        : UartCommandBase(
            static_cast<uint8_t>(COMMANDS::OI::SHA1),
            COMMANDS::SHA1::COMMAND_LENGTH)
    {
        m_payload.at(offsetof(COMMANDS::SHA1::command_t, OL)) = data.size();
        m_payload.resize(data.size() + 2);

        for (int i = 0; i < data.size(); i++) {
            m_payload.at(offsetof(COMMANDS::SHA1::command_t, data) + i)
                = data.at(i);
        }
    };

    void print(std::ostream& out) const override
    {
        uint8_t c;
        out << "SHA1          : ";
        UartCommandBase::print(out);
    };

    COMMANDS::SHA1::response_t responseStruct()
    {
        return {(uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH]};
    };
};

