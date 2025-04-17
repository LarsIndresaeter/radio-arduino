#pragma once

#include <common/uartCommandBase.hpp>

class UartCommandDebug : public UartCommandBase {
public:
    UartCommandDebug()
        : UartCommandBase(
            static_cast<uint8_t>(COMMANDS::OI::DEBUG),
            COMMANDS::DEBUG::COMMAND_LENGTH) {};

    void print(std::ostream& out) const override
    {
        out << "DEBUG         : ";
        UartCommandBase::print(out);
    };

    COMMANDS::DEBUG::response_t responseStruct()
    {
        if(m_response.size() >= sizeof(COMMANDS::DEBUG::response_t))
        {
            return {(uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH]};
        }
        else{
            return((uint8_t*){});
        }
    }
};

