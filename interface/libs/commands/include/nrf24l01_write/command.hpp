#pragma once

#include <common/uartCommandBase.hpp>

class UartCommandNrf24l01Write : public UartCommandBase {
public:
    UartCommandNrf24l01Write(std::vector<uint8_t> data)
        : UartCommandBase(
            static_cast<uint8_t>(COMMANDS::OI::NRF24L01_WRITE),
            COMMANDS::NRF24L01_WRITE::COMMAND_LENGTH)
    {
        COMMANDS::NRF24L01_WRITE::command_t command;

        if (data.size() < sizeof(command.data)) {
            m_payload.at(offsetof(COMMANDS::NRF24L01_WRITE::command_t, length))
                = data.size();
        }
        else {
            m_payload.at(offsetof(COMMANDS::NRF24L01_WRITE::command_t, length))
                = sizeof(command.data);
        }

        for (int i = 0; i < data.size(); i++) {
            if (i >= sizeof(command.data)) {
                std::cout << "WARNING: NRF24L01 payload truncated after "
                          << static_cast<int>(
                                 sizeof(command.data))
                          << " bytes" << std::endl;
                break;
            }
            m_payload.at(offsetof(COMMANDS::NRF24L01_WRITE::command_t, data[0]) + i)
                = data.at(i);
        }
    };

    void print(std::ostream& out) const override
    {
        COMMANDS::NRF24L01_WRITE::response_t response(
            (uint8_t*)&m_response.data()[4]);
        out << "NRF24L01_WRITE  : ";
        if(response.status == 1)
        {
            out << "OK";
        }
        else
        {
            out << "FAILED";
        }

        out << std::setfill('0') << std::setw(2) << std::hex << std::uppercase;

        for (int i = 0;
             (i < response.length) && (i < sizeof(response.data));
             i++) {
            out << " 0x" << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << static_cast<int>(response.data[i]);
        }
        out << " ]";
    };

    std::string getString()
    {
        std::string retval;

        COMMANDS::NRF24L01_WRITE::response_t r = responseStruct();

        for(int i=0;i<r.length;i++)
        {
            retval.push_back(r.data[i]);
        }

        return retval;
    }

    std::string getStatusString()
    {
        std::string retval = "........ ";

        COMMANDS::NRF24L01_WRITE::response_t r = responseStruct();

        if(r.length > 0)
        {
            retval.at(0) = r.data[0];
        }

        if(r.length > 32)
        {
            retval.at(2) = r.data[32];
        }

        if(r.length > 64)
        {
            retval.at(4) = r.data[64];
        }

        if(r.length > 96)
        {
            retval.at(6) = r.data[96];
        }

        retval = retval + std::to_string(static_cast<int>(r.length));

        return retval;
    }

    COMMANDS::NRF24L01_WRITE::response_t responseStruct()
    {
        return { (uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH] };
    };
};

