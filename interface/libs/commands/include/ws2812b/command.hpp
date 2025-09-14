#pragma once

#include <common/uartCommandBase.hpp>
#include <cstdint>

class UartCommandWs2812b : public UartCommandBase {
public:
    UartCommandWs2812b()
        : UartCommandBase(
            static_cast<uint8_t>(COMMANDS::OI::WS2812B),
            COMMANDS::WS2812B::COMMAND_LENGTH)
    {
    };

    void print(std::ostream& out) const override
    {
        out << "WS2812B : ";
    };

    COMMANDS::WS2812B::response_t responseStruct()
    {
        return {(uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH]};
    };

    void setLed(uint8_t pos, uint8_t red, uint8_t green, uint8_t blue)
    {
        m_payload.at(offsetof(COMMANDS::WS2812B::command_t, red) + pos) = red;
        m_payload.at(offsetof(COMMANDS::WS2812B::command_t, green) + pos) = green;
        m_payload.at(offsetof(COMMANDS::WS2812B::command_t, blue) + pos) = blue;
    }
};

