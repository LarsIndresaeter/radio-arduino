#pragma once

#include <common/uartCommandBase.hpp>
#include <cstdint>

class UartCommandTimer : public UartCommandBase {
public:
    UartCommandTimer()
        : UartCommandBase(
            static_cast<uint8_t>(COMMANDS::OI::TIMER),
            COMMANDS::TIMER::COMMAND_LENGTH) {};

    void print(std::ostream& out) const override
    {
        COMMANDS::TIMER::response_t response(
            (uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH]);

        out << "TIMER         : ";

        uint16_t pulse_width = response.pulse_width_high << 8;
        pulse_width |= response.pulse_width_low;
        out << " pulse_width=" << std::dec << static_cast<int>(pulse_width) << " us";
    };

    COMMANDS::TIMER::response_t responseStruct()
    {
        return {(uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH]};
    }
};

