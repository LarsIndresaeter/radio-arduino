#pragma once
// This file is generated with the script: `interface/libs/commands/generate.py`

#include <common/uartCommandBase.hpp>

class UartCommandGetDeviceInfo : public UartCommandBase {
public:
    UartCommandGetDeviceInfo()
        : UartCommandBase(
              static_cast<uint8_t>(COMMANDS::OI::GET_DEVICE_INFO),
              COMMANDS::GET_DEVICE_INFO::COMMAND_LENGTH)
    {
        COMMANDS::GET_DEVICE_INFO::command_t command;

    };

    void printResponse(std::ostream& out, COMMANDS::GET_DEVICE_INFO::response_t response) const
    {
        out << "GET_DEVICE_INFO        : ";
        out << " nameString=\"";
        for (uint8_t i = 0; i < 16; i++) {
            if(response.nameString[i])
            {
                out << static_cast<char>(response.nameString[i]);
            }
        }
        out << "\"";
        out << " versionString=\"";
        for (uint8_t i = 0; i < 32; i++) {
            if(response.versionString[i])
            {
                out << static_cast<char>(response.versionString[i]);
            }
        }
        out << "\"";
        out << " status=" << static_cast<int>(response.getStatus());
    }

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::GET_DEVICE_INFO::RESPONSE_LENGTH + 4)) {
            COMMANDS::GET_DEVICE_INFO::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        }
        else {
            std::cout << "GET_DEVICE_INFO: insufficient data" << std::endl;
        }
    };

    COMMANDS::GET_DEVICE_INFO::response_t responseStruct()
    {
        COMMANDS::GET_DEVICE_INFO::response_t response;

        if (m_responsePayload.size() >= sizeof(response)) {
            return { (uint8_t*)&m_responsePayload[0] };
        }

        return (response);
    };
};

