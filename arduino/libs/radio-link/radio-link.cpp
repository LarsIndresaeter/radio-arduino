#include <radio-link.hpp>

extern bool rx_mode_gateway;

namespace RADIOLINK {

uint8_t node_address = 0;
uint8_t rx_tx_addr[5] = { 0xF0, 0xF0, 0xF0, 0xF0, node_address };
uint32_t id_this_node = 0;
uint32_t id_from_last_discover_message = 0;

uint32_t getLastDeviceIdSeen()
{
    uint32_t tmp = id_from_last_discover_message;
    id_from_last_discover_message = 0;
    return (tmp);
}

void setDeviceId(uint32_t id) { id_this_node = id; }

// link level package
constexpr uint8_t discovery_package_prototye[32]
    = { 'r', 'a', 'd', 'i', 'o', '-', 'a', 'r', 'd', 'u', 'i', 'n', 'o', ' ', 'd', 'i',
        's', 'c', 'o', 'v', 'e', 'r', 'y', 0,   0,   0,   0,   0,   0,   0,   0,   0 };

// link level package
constexpr uint8_t wakeup_package_prototype[32]
    = { 'r', 'a', 'd', 'i', 'o', '-', 'a', 'r', 'd', 'u', 'i', 'n', 'o', ' ', 'w', 'a',
        'k', 'e', 'u', 'p', 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 };

void populateDiscoverPackage(uint8_t* buffer, uint8_t attentionFlag)
{
    for (uint8_t i = 0; i < 27; i++) {
        buffer[i] = discovery_package_prototye[i];
    }

    buffer[27] = attentionFlag;
    buffer[28] = id_this_node >> 24;
    buffer[29] = id_this_node >> 16;
    buffer[30] = id_this_node >> 8;
    buffer[31] = id_this_node;
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

void setNodeAddress(uint8_t address)
{
    node_address = address;

    rx_tx_addr[NRF24L01_ADDR_SIZE - 1] = address;

    NRF24L01_init(&rx_tx_addr[0], &rx_tx_addr[0], rx_mode_gateway);
}

uint8_t sendDiscoverToGateway()
{
    uint8_t wakeup_received_from_gateway = 0;
    uint8_t read_wakeup_command[32] = { 0 };

    uint8_t rf_link_discover_package[32];
    populateDiscoverPackage(rf_link_discover_package, QUADENCODER::pollChangedFlag());

    uint8_t rf_link_wakeup_command[32];
    populateWakeupPackage(rf_link_wakeup_command, id_this_node);

    // send command to rf gateway
    NRF24L01_tx(&rf_link_discover_package[0], 32);

    _delay_ms(10);

    // poll gateway for wakeup command in ack packet
    uint8_t length = NRF24L01_read_rx_payload(&read_wakeup_command[0]);

    // check if wakeup command was sent to me
    if (length == 32) {
        wakeup_received_from_gateway = 1;
        //for (uint8_t i = 0; i < 32; i++) {
        for (uint8_t i = 0; i < 25; i++) {
            if (read_wakeup_command[i] != rf_link_wakeup_command[i]) {
                wakeup_received_from_gateway = 0;
            }
        }
    }

    if (wakeup_received_from_gateway) {
        QUADENCODER::clearChangedFlag();
    }

    return wakeup_received_from_gateway;
}

uint8_t sendWakeupCommandToNode(uint32_t id, uint8_t checkAttentionFlag)
{
    uint8_t read_discover_package[32] = { 0 };
    uint8_t discovered = 0;

    uint8_t rf_link_wakeup_command[32];

    if (id == 0) {
        id = id_from_last_discover_message;
    }

    populateWakeupPackage(rf_link_wakeup_command, id);

    if (rx_mode_gateway) {
        for (uint16_t i = 0; i < 500; i++) {
            uint8_t length = NRF24L01_read_rx_payload(&read_discover_package[0]);

            if (length == 32) {
                if ((0 != checkAttentionFlag) && (0 == read_discover_package[31])) {
                    // received discover package but about wakeup since data available flag was not set
                    break;
                }
                else {
                    discovered = 1;
                    NRF24L01_tx(&rf_link_wakeup_command[0], 32);

                    for (uint8_t j = 0; j < 31; j++) {
                        if (read_discover_package[j] != rf_link_wakeup_command[j]) {
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

    return discovered;
}

uint8_t isDiscoverPackage(uint8_t response_length, uint8_t* packet)
{
    uint8_t rf_link_discover_package[32];
    populateDiscoverPackage(rf_link_discover_package, QUADENCODER::pollChangedFlag());

    // ignore messages from rx node if it is a wakeup ack packet
    uint8_t is_wakeup_ack = 0;
    if (response_length == 32) {
        is_wakeup_ack = 1;
        for (uint8_t j = 0; j < 25; j++) {
            if (packet[j] != rf_link_discover_package[j]) {
                is_wakeup_ack = 0;
            }
        }
    }

    if (is_wakeup_ack == 1) {
        id_from_last_discover_message = packet[31];
        id_from_last_discover_message += ((uint32_t)packet[30]) << 8;
        id_from_last_discover_message += ((uint32_t)packet[29]) << 16;
        id_from_last_discover_message += ((uint32_t)packet[28]) << 24;
    }

    return is_wakeup_ack;
}
} // namespace

