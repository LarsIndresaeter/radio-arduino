#pragma once

#include <common/uartCommandBase.hpp>

class UartCommandDs18b20 : public UartCommandBase {
public:
    UartCommandDs18b20()

        : UartCommandBase(
            static_cast<uint8_t>(COMMANDS::OI::DS18B20),
            COMMANDS::DS18B20::COMMAND_LENGTH) {};

    void print(std::ostream& out) const override
    {
        COMMANDS::DS18B20::response_t response(
            (uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH]);

        uint16_t temperature = ((uint16_t)response.temperature[0]) << 8
            | response.temperature[1];
        float temperatureFloat = 0.0;
        if (temperature > 0) {
            temperatureFloat = temperature / 16.0;
        }

        out << "DS18B20  : ";
        out << " " << temperatureFloat << " ";
    };

    COMMANDS::DS18B20::response_t responseStruct()

    {
        return { (uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH] };
    };
};
