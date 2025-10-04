#include <radio_link.hpp>

#ifdef REPLACE_UART_WITH_RADIO_COMMUNICATION_AKA_RX_NODE
constexpr bool rx_mode_gateway = false;
#else
constexpr bool rx_mode_gateway = true;
#endif

uint8_t rx_tx_addr[5] = { 0xF0, 0xF0, 0xF0, 0xF0, node_address };
uint8_t node_address = 0;

uint8_t rf_link_discover_package[32]
    = { 'd', 'i', 's', 'c', 'o', 'v', 'e', 'r', ' ', 0xaa, 0xaa,
          0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
          0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, node_address, 1 };

uint8_t rf_link_wakeup_command[32]
    = { 'w', 'a', 'k', 'e', 'u', 'p', ' ', 0x55, 0x55, 0x55, 0x55,
          0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
          0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, node_address };

uint8_t attention_flag = 0;

uint8_t sendDiscoverToGateway()
{
    uint8_t wakeup_received_from_gateway = 0;
    uint8_t read_wakeup_command[32] = { 0 };

    // send command to rf gateway
    rf_link_discover_package[31] = attention_flag;
    NRF24L01_tx(&rf_link_discover_package[0], 32);

    _delay_ms(10);

    // poll gateway for wakeup command in ack packet
    uint8_t length = NRF24L01_read_rx_payload(&read_wakeup_command[0]);

    if (length == 32) {
        wakeup_received_from_gateway = 1;
        for (uint8_t i = 0; i < 32; i++) {
            if (read_wakeup_command[i] != rf_link_wakeup_command[i]) {
                wakeup_received_from_gateway = 0;
            }
        }
    }

    return wakeup_received_from_gateway;
}

uint8_t wakeupCommand(uint8_t checkAttentionFlag)
{
    uint8_t read_discover_package[32] = { 0 };
    uint8_t attention_flag = 0;

    if (rx_mode_gateway) {
        for (uint16_t i = 0; i < 1000; i++) {
            uint8_t length = NRF24L01_read_rx_payload(&read_discover_package[0]);

            if (length == 32) {
                attention_flag = read_discover_package[31];

                if ((0 != checkAttentionFlag) && (0 == read_discover_package[31])) {
                    // received discover package but about wakeup since data available flag was not set
                    break;
                }
                else {
                    NRF24L01_tx(&rf_link_wakeup_command[0], 32);

                    for (uint8_t j = 0; j < 31; j++) {
                        if (read_discover_package[j] != rf_link_discover_package[j]) {
                            // set status
                        }
                        i = 10000;
                        break;
                    }
                }
            }

            _delay_ms(10);
        }
        _delay_ms(10); // give rf node some time to be ready for new commands
    }

    return attention_flag;
}

uint8_t is_discover_package(uint8_t response_length, uint8_t* packet)
{
    // ignore messages from rx node if it is a wakeup ack packet
    uint8_t is_wakeup_ack = 0;
    if (response_length == 32) {
        is_wakeup_ack = 1;
        for (uint8_t j = 0; j < 31; j++) {
            if (packet[j] != rf_link_discover_package[j]) {
                is_wakeup_ack = 0;
            }
        }
    }

    return is_wakeup_ack;
}
