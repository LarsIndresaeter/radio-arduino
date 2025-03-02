#pragma once

#include <common/uartCommandBase.hpp>

class UartCommandAes : public UartCommandBase {
public:
    UartCommandAes(uint8_t d, std::vector<uint8_t> data)
        : UartCommandBase(
            static_cast<uint8_t>(COMMANDS::OI::AES),
            COMMANDS::AES::COMMAND_LENGTH)
    {
        m_payload.at(offsetof(COMMANDS::AES::command_t, type)) = d;

        for (int i = 0; i < data.size(); i++) {
            m_payload.at(offsetof(COMMANDS::AES::command_t, data) + i)
                = data.at(i);
        }
    };

    void print(std::ostream& out) const override
    {
        COMMANDS::AES::response_t response((uint8_t*)&m_response.data()[4]);
        uint8_t c;
        out << "AES               : ";
        if (response.type == 'd') {
            std::cout << "decrypt: ";
        }
        if (response.type == 'c') {
            std::cout << "encrypt: ";
        }
        UartCommandBase::print(out);
    };

    COMMANDS::AES::response_t responseStruct()

    {
        return {(uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH]};
    };
};

