#pragma once

#include <common/uartCommandBase.hpp>

class UartCommandNrf24l01Init : public UartCommandBase {
public:
    UartCommandNrf24l01Init(std::vector<uint8_t> txAddr, std::vector<uint8_t> rxAddr, uint8_t rfChannel, uint8_t gateway)
        : UartCommandBase(
            static_cast<uint8_t>(COMMANDS::OI::NRF24L01_INIT),
            COMMANDS::NRF24L01_INIT::COMMAND_LENGTH)
    {
        COMMANDS::NRF24L01_INIT::command_t command;

        for (int i = 0; i < txAddr.size(); i++) {
            if (i >= sizeof(command.txAddr)) {
                break;
            }
            m_payload.at(
                offsetof(COMMANDS::NRF24L01_INIT::command_t, txAddr[0]) + i)
                = txAddr.at(i);
        }

        for (int i = 0; i < rxAddr.size(); i++) {
            if (i >= sizeof(command.rxAddr)) {
                break;
            }
            m_payload.at(
                offsetof(COMMANDS::NRF24L01_INIT::command_t, rxAddr[0]) + i)
                = rxAddr.at(i);
        }

        m_payload.at(offsetof(COMMANDS::NRF24L01_INIT::command_t, rfChannel)) = rfChannel;

        m_payload.at(offsetof(COMMANDS::NRF24L01_INIT::command_t, gateway)) = gateway;

    };

    void printResponse(std::ostream& out, COMMANDS::NRF24L01_INIT::response_t response) const
    {
        out << "NRF24L01_INIT   : ";
    };

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::NRF24L01_INIT::RESPONSE_LENGTH + 4)) {
            COMMANDS::NRF24L01_INIT::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        } else
        {
            std::cout << "NRF24L01_INIT: insufficient data" << std::endl;
        }
    };

    COMMANDS::NRF24L01_INIT::response_t responseStruct()
    {
        return { (uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH] };
    };
};

