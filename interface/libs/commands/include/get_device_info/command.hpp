#pragma once

#include <common/uartCommandBase.hpp>

class UartCommandGetDeviceInfo : public UartCommandBase {
public:
    UartCommandGetDeviceInfo()
        : UartCommandBase(
            static_cast<uint8_t>(COMMANDS::OI::GET_DEVICE_INFO),
            COMMANDS::GET_DEVICE_INFO::COMMAND_LENGTH) {};

    void print(std::ostream& out) const override
    {
        COMMANDS::GET_DEVICE_INFO::response_t response(
            (uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH]);

        out << "GET_DEVICE_INFO       : name=";

        for (int i = 0; i < 16 && response.name[i] != 0; i++) {
            out << response.name[i];
        }

        out << " , version=";

        for (int i = 0; i < 32 && response.version[i] != 0; i++) {
            out << response.version[i];
        }
    };

    COMMANDS::GET_DEVICE_INFO::response_t responseStruct()
    {
        return { (uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH] };
    }
};

