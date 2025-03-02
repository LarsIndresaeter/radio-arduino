#pragma once

#include <common/uartCommandBase.hpp>

class UartCommandGpio : public UartCommandBase {
public:
    UartCommandGpio()
        : UartCommandBase(
            static_cast<uint8_t>(COMMANDS::OI::GPIO),
            COMMANDS::GPIO::COMMAND_LENGTH)
    {
    };

    void print(std::ostream& out) const override
    {
        COMMANDS::GPIO::response_t response(
            (uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH]);

        out << "GPIO          : ";
        out << "portB=" << static_cast<int>(response.portB)
            << ", portC=" << static_cast<int>(response.portC)
            << ", portD=" << static_cast<int>(response.portD);
    };

    COMMANDS::GPIO::response_t responseStruct()
    {
        return {(uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH]};
    };
};

