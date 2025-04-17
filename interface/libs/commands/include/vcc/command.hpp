#pragma once

#include <common/uartCommandBase.hpp>

class UartCommandVcc : public UartCommandBase {
public:
    UartCommandVcc()
        : UartCommandBase(
            static_cast<uint8_t>(COMMANDS::OI::VCC),
            COMMANDS::VCC::COMMAND_LENGTH) {};

    void print(std::ostream& out) const override
    {
        COMMANDS::VCC::response_t response(
            (uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH]);

        uint16_t vcc = (response.vcc_h << 8) | response.vcc_l;
        out << "VCC             : " << std::dec << static_cast<int>(vcc) << " mV";
    };

    COMMANDS::VCC::response_t responseStruct()
    {
        return {(uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH]};
    };
};

