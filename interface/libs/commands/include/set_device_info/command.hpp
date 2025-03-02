#pragma once

#include <common/uartCommandBase.hpp>

class UartCommandSetDeviceInfo : public UartCommandBase {
public:
    UartCommandSetDeviceInfo(std::vector<uint8_t> data)
        : UartCommandBase(
            static_cast<uint8_t>(COMMANDS::OI::SET_DEVICE_INFO),
            COMMANDS::SET_DEVICE_INFO::COMMAND_LENGTH)
    {
        for (int i = 0; i < data.size() & i < 16; i++) {
            m_payload.at(offsetof(COMMANDS::SET_DEVICE_INFO::command_t, name) + i)
                = data.at(i);
        }
    };

    void print(std::ostream& out) const override
    {
        out << "SET_DEVICE_INFO       : ";
        UartCommandBase::print(out);
    };

    COMMANDS::SET_DEVICE_INFO::response_t responseStruct()
    {
        return { (uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH] };
    }
};

