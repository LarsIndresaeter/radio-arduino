#pragma once
// This file is generated with the script: `interface/libs/commands/generate.py`

#include <common/uartCommandBase.hpp>

class UartCommandSetDeviceInfo : public UartCommandBase {
public:
    UartCommandSetDeviceInfo(std::vector<uint8_t> name)
        : UartCommandBase(
            static_cast<uint8_t>(COMMANDS::OI::SET_DEVICE_INFO),
            COMMANDS::SET_DEVICE_INFO::COMMAND_LENGTH)
    {
        COMMANDS::SET_DEVICE_INFO::command_t command;

        for (int i = 0; i < name.size(); i++) {
            if (i >= sizeof(command.name)) {
                break;
            }
            m_payload.at(
                offsetof(COMMANDS::SET_DEVICE_INFO::command_t, name[0]) + i)
                = name.at(i);
        }

    };

    void printResponse(std::ostream& out, COMMANDS::SET_DEVICE_INFO::response_t response) const
    {
        out << "SET_DEVICE_INFO        : ";
        out << " status=" << static_cast<int>(response.getStatus());
    }

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::SET_DEVICE_INFO::RESPONSE_LENGTH + 4)) {
            COMMANDS::SET_DEVICE_INFO::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        } else
        {
            std::cout << "SET_DEVICE_INFO: insufficient data" << std::endl;
        }
    };

    COMMANDS::SET_DEVICE_INFO::response_t responseStruct()
    {
        return { (uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH] };
    };
};

