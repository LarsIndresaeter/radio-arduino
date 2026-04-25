#pragma once

#include <nrf24l01.hpp>
#include <quadencoder.hpp>
#include <stdint.h>
#include <util/delay.h>

namespace RADIOLINK {

constexpr uint8_t advertisement_payload_data_size = 16;

// PDU = Protocol Data Unit
typedef struct advertisement_payload {
    uint8_t reserverd_for_future_use;
    uint8_t flags;
    uint8_t sequence_counter;
    uint8_t rfu;
    uint32_t id;
    uint8_t data[advertisement_payload_data_size];
    uint32_t crc;
} advertisement_payload_t;

// link level package
constexpr uint8_t advertisement_package_prototype[32]
    = { 0xFE, 0xED, 0x80, 28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

constexpr uint8_t offset_advertisement_pdu = 4;
constexpr uint8_t offset_advertisement_flags = offset_advertisement_pdu + 1;
constexpr uint8_t offset_advertisement_sequence_counter = offset_advertisement_pdu + 2;
constexpr uint8_t offset_advertisement_id = offset_advertisement_pdu + 6;
constexpr uint8_t offset_advertisement_data = offset_advertisement_pdu + 10;
constexpr uint8_t offset_advertisement_crc = offset_advertisement_pdu + 26;

static_assert(
    offset_advertisement_pdu + sizeof(advertisement_payload_t) == 32,
    "advertisement pdu does not line up with radio packet");

// link level package
constexpr uint8_t wakeup_package_prototype[32]
    = { 0xFE, 0xED, 0xC0, 28,  'r', 'a', 'd', 'i', 'o', '-', 'a', 'r', 'd', 'u', 'i', 'n',
        'o',  ' ',  'w',  'a', 'k', 'e', 'u', 'p', 0,   0,   0,   0,   0,   0,   0,   0 };

void setDestinationAddress(uint32_t id);
uint32_t getLastDeviceIdSeen();
uint8_t sendAdvertisementToGateway();
uint8_t sendWakeupCommandToNode(uint32_t id, uint8_t checkAttentionFlag);
uint8_t isAdvertisementPackage(uint8_t response_length, uint8_t* _packet);
void initRadioLink();

void closeCommunicationPipe();
void broadcast_tx(uint8_t* rx_buffer);

} // namespace
