#include <radio-link.hpp>

extern bool rx_mode_gateway;

namespace RADIOLINK {

uint8_t node_address = 0;
uint8_t rx_tx_addr[5] = { 0xF0, 0xF0, 0xF0, 0xF0, node_address };
uint8_t device_id[4] = { 0 };
uint32_t lastDeviceIdSeen = 0;

uint32_t getLastDeviceIdSeen()
{
    uint32_t tmp = lastDeviceIdSeen;
    lastDeviceIdSeen = 0;
    return (tmp);
}

void setDeviceId(uint32_t id)
{
    device_id[0] = id >> 24;
    device_id[1] = id >> 16;
    device_id[2] = id >> 8;
    device_id[3] = id;
}

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
    buffer[28] = device_id[0];
    buffer[29] = device_id[1];
    buffer[30] = device_id[2];
    buffer[31] = device_id[3];
}

void populateWakeupPackage(uint8_t* buffer)
{
    for (uint8_t i = 0; i < 28; i++) {
        buffer[i] = wakeup_package_prototype[i];
    }

    buffer[28] = device_id[0];
    buffer[29] = device_id[1];
    buffer[30] = device_id[2];
    buffer[31] = device_id[3];
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
    populateWakeupPackage(rf_link_wakeup_command);

    // send command to rf gateway
    NRF24L01_tx(&rf_link_discover_package[0], 32);

    _delay_ms(10);

    // poll gateway for wakeup command in ack packet
    uint8_t length = NRF24L01_read_rx_payload(&read_wakeup_command[0]);

    if (length == 32) {
        wakeup_received_from_gateway = 1;
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

uint8_t sendWakeupCommandToNode(uint8_t checkAttentionFlag)
{
    uint8_t read_discover_package[32] = { 0 };
    uint8_t discovered = 0;

    uint8_t rf_link_wakeup_command[32];
    populateWakeupPackage(rf_link_wakeup_command);

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
        lastDeviceIdSeen = packet[31];
        lastDeviceIdSeen += ((uint32_t)packet[30]) << 8;
        lastDeviceIdSeen += ((uint32_t)packet[29]) << 16;
        lastDeviceIdSeen += ((uint32_t)packet[28]) << 24;
    }

    return is_wakeup_ack;
}
} // namespace

