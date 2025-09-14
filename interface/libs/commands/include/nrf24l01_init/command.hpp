#pragma once

#include <common/uartCommandBase.hpp>

class UartCommandNrf24l01Init : public UartCommandBase {
public:
    UartCommandNrf24l01Init(
        std::vector<uint8_t> tx_addr,
        std::vector<uint8_t> rx_addr,
        uint8_t rf_channel,
        bool gateway)
        : UartCommandBase(
            static_cast<uint8_t>(COMMANDS::OI::NRF24L01_INIT),
            COMMANDS::NRF24L01_INIT::COMMAND_LENGTH)
    {
        COMMANDS::NRF24L01_INIT::command_t command;

        m_payload.at(offsetof(COMMANDS::NRF24L01_INIT::command_t, rf_channel))
            = rf_channel;
        if (gateway) {
            m_payload.at(offsetof(COMMANDS::NRF24L01_INIT::command_t, gateway))
                = 0x01;
        }
        else {
            m_payload.at(offsetof(COMMANDS::NRF24L01_INIT::command_t, gateway))
                = 0x00;
        }

        for (int i = 0; i < tx_addr.size(); i++) {
            if (i >= sizeof(command.tx_addr)) {
                std::cout << "WARNING: NRF24L01 payload truncated after "
                          << static_cast<int>(
                                 sizeof(command.tx_addr))
                          << " bytes" << std::endl;
                break;
            }
            m_payload.at(
                offsetof(COMMANDS::NRF24L01_INIT::command_t, tx_addr[0]) + i)
                = tx_addr.at(i);
        }

        for (int i = 0; i < rx_addr.size(); i++) {
            if (i >= sizeof(command.rx_addr)) {
                std::cout << "WARNING: NRF24L01 payload truncated after "
                          << static_cast<int>(
                                 sizeof(command.rx_addr))
                          << " bytes" << std::endl;
                break;
            }
            m_payload.at(
                offsetof(COMMANDS::NRF24L01_INIT::command_t, rx_addr[0]) + i)
                = rx_addr.at(i);
        }
    };

    void print(std::ostream& out) const override
    {
        COMMANDS::NRF24L01_INIT::response_t response(
            (uint8_t*)&m_response.data()[4]);
        out << "nRF24L01_INIT  :";
    };

    COMMANDS::NRF24L01_INIT::response_t responseStruct()
    {
        return { (uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH] };
    };
};

