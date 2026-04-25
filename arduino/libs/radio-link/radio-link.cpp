#include "include/radio-link.hpp"
#include <radio-link.hpp>

extern bool rx_mode_gateway;

namespace RADIOLINK {

uint8_t rx_tx_addr[5] = { 0xF0, 0xF0, 0xF0, 0xF0, 0 };
uint32_t destination_address = 0;
uint32_t id_from_last_advertisement_message = 0;

uint32_t getLastDeviceIdSeen()
{
    uint32_t tmp = id_from_last_advertisement_message;
    id_from_last_advertisement_message = 0;
    closeCommunicationPipe();
    return (tmp);
}

uint8_t advertisement_sequence_counter = 0;

void populateAdvertisementPackage(uint8_t* buffer, uint8_t attentionFlag)
{
    for (uint8_t i = 0; i < 27; i++) {
        buffer[i] = advertisement_package_prototype[i];
    }

    buffer[offset_advertisement_flags] = attentionFlag;
    buffer[offset_advertisement_sequence_counter] = advertisement_sequence_counter++;
    buffer[offset_advertisement_id] = destination_address >> 24;
    buffer[offset_advertisement_id + 1] = destination_address >> 16;
    buffer[offset_advertisement_id + 2] = destination_address >> 8;
    buffer[offset_advertisement_id + 3] = destination_address;
}

void populateWakeupPackage(uint8_t* buffer, uint32_t id)
{
    for (uint8_t i = 0; i < 28; i++) {
        buffer[i] = wakeup_package_prototype[i];
    }

    buffer[28] = id >> 24;
    buffer[29] = id >> 16;
    buffer[30] = id >> 8;
    buffer[31] = id;
}

void setupCommunicationPipe()
{
    rx_tx_addr[0] = 0xF1;
    rx_tx_addr[1] = destination_address >> 24;
    rx_tx_addr[2] = destination_address >> 16;
    rx_tx_addr[3] = destination_address >> 8;
    rx_tx_addr[4] = destination_address;

    NRF24L01_write_register(NRF24L01_REGISTER_RX_ADDR_P0, &rx_tx_addr[0], NRF24L01_ADDR_SIZE);
    NRF24L01_write_register(NRF24L01_REGISTER_TX_ADDR, &rx_tx_addr[0], NRF24L01_ADDR_SIZE);
}

void closeCommunicationPipe()
{
    rx_tx_addr[0] = 0xF0;
    rx_tx_addr[1] = 0xF0;
    rx_tx_addr[2] = 0xF0;
    rx_tx_addr[3] = 0xF0;
    rx_tx_addr[4] = 0;

    if (rx_mode_gateway) {
        NRF24L01_write_register(NRF24L01_REGISTER_RX_ADDR_P1, &rx_tx_addr[0], NRF24L01_ADDR_SIZE);
    }

    NRF24L01_write_register(NRF24L01_REGISTER_RX_ADDR_P0, &rx_tx_addr[0], NRF24L01_ADDR_SIZE);
    NRF24L01_write_register(NRF24L01_REGISTER_TX_ADDR, &rx_tx_addr[0], NRF24L01_ADDR_SIZE);
}

void setupBroadcastPipe()
{
    rx_tx_addr[0] = 0xF0;
    rx_tx_addr[1] = 0xF0;
    rx_tx_addr[2] = 0xF0;
    rx_tx_addr[3] = 0xF0;
    rx_tx_addr[4] = 0;

    if (rx_mode_gateway) {
        NRF24L01_write_register(NRF24L01_REGISTER_RX_ADDR_P1, &rx_tx_addr[0], NRF24L01_ADDR_SIZE);
    }
    else {
        NRF24L01_write_register(NRF24L01_REGISTER_RX_ADDR_P0, &rx_tx_addr[0], NRF24L01_ADDR_SIZE);
    }

    NRF24L01_write_register(NRF24L01_REGISTER_TX_ADDR, &rx_tx_addr[0], NRF24L01_ADDR_SIZE);
}

void initRadioLink()
{
    rx_tx_addr[0] = 0xF0;
    rx_tx_addr[1] = 0xF0;
    rx_tx_addr[2] = 0xF0;
    rx_tx_addr[3] = 0xF0;
    rx_tx_addr[4] = 0;

    NRF24L01_init(&rx_tx_addr[0], &rx_tx_addr[0], rx_mode_gateway);
    setupBroadcastPipe();
}

void setDestinationAddress(uint32_t id) { destination_address = id; }

void broadcast_tx(uint8_t* buffer)
{
    setupBroadcastPipe();
    NRF24L01_write_tx_payload_no_ack(buffer, 32);
    setupCommunicationPipe();
}

uint8_t sendAdvertisementToGateway()
{
    uint8_t wakeup_received_from_gateway = 0;
    uint8_t read_wakeup_command[32] = { 0 };

    uint8_t rf_link_advertisement_package[32];
    populateAdvertisementPackage(rf_link_advertisement_package, QUADENCODER::pollChangedFlag());

    uint8_t rf_link_wakeup_command[32];
    populateWakeupPackage(rf_link_wakeup_command, destination_address);

    broadcast_tx(&rf_link_advertisement_package[0]);

    _delay_ms(20);

    uint8_t length = NRF24L01_read_rx_payload(&read_wakeup_command[0]);

    // check if wakeup command was sent to me
    if (length == 32) {
        wakeup_received_from_gateway = 1;
        for (uint8_t i = 0; i < 32; i++) {
            if (read_wakeup_command[i] != rf_link_wakeup_command[i]) {
                wakeup_received_from_gateway = 0;
            }
        }
    }

    if (wakeup_received_from_gateway == 1) {
        QUADENCODER::clearChangedFlag();
    }

    return wakeup_received_from_gateway;
}

uint8_t sendWakeupCommandToNode(uint32_t id, uint8_t checkAttentionFlag)
{
    uint8_t discovered = 0;

    setDestinationAddress(id);

    setupBroadcastPipe();

    if (rx_mode_gateway) {
        for (uint16_t i = 0; i < 1000; i++) {
            uint8_t read_advertisement_package[32] = { 0 };
            uint8_t length = NRF24L01_read_rx_payload(&read_advertisement_package[0]);
            discovered = isAdvertisementPackage(length, read_advertisement_package);

            if ((discovered == 1) && (id_from_last_advertisement_message == id)) {
                if ((0 != checkAttentionFlag) && (0 == read_advertisement_package[offset_advertisement_flags])) {
                    // received advertisement package but abort wakeup since data available flag was not set
                }
                else {
                    uint8_t rf_link_wakeup_command[32];
                    populateWakeupPackage(rf_link_wakeup_command, id);
                    setupCommunicationPipe();
                    _delay_ms(1);
                    NRF24L01_write_tx_payload_no_ack(rf_link_wakeup_command, 32);
                }
                i = 10000;
            }

            _delay_ms(5);
        }
    }

    return discovered;
}

uint8_t isAdvertisementPackage(uint8_t response_length, uint8_t* packet)
{
    uint8_t rf_link_advertisement_package[32];
    populateAdvertisementPackage(rf_link_advertisement_package, 0);

    uint8_t is_wakeup_ack = 0;

    if (response_length == 32) {
        is_wakeup_ack = 1;
        for (uint8_t j = 0; j < offset_advertisement_pdu; j++) {
            if (packet[j] != rf_link_advertisement_package[j]) {
                is_wakeup_ack = 0;
            }
        }
    }

    if (is_wakeup_ack == 1) {
        id_from_last_advertisement_message = ((uint32_t)packet[offset_advertisement_id]) << 24;
        id_from_last_advertisement_message += ((uint32_t)packet[offset_advertisement_id + 1]) << 16;
        id_from_last_advertisement_message += ((uint32_t)packet[offset_advertisement_id + 2]) << 8;
        id_from_last_advertisement_message += ((uint32_t)packet[offset_advertisement_id + 3]);
    }

    return is_wakeup_ack;
}
} // namespace
