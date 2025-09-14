#pragma once

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
        out << "GET_DEVICE_INFO   : ";

        out << "name=";
        //seher

        for (int i = 0; i < sizeof(response.name) && response.name[i] != 0; i++) {
            out << response.name[i];
        }

        out << " , version=";

        for (int i = 0; i < sizeof(response.version) && response.version[i] != 0; i++) {
            out << response.version[i];
        }
    };

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::GET_DEVICE_INFO::RESPONSE_LENGTH + 4)) {
            COMMANDS::GET_DEVICE_INFO::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        } else
        {
            std::cout << "GET_DEVICE_INFO: insufficient data" << std::endl;
        }
    };

    COMMANDS::GET_DEVICE_INFO::response_t responseStruct()
    {
        return { (uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH] };
    };
};

