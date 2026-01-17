#pragma once

#include <nrf24l01.hpp>
#include <quadencoder.hpp>
#include <stdint.h>
#include <util/delay.h>

namespace RADIOLINK {
void setDeviceId(uint32_t id);
uint32_t getLastDeviceIdSeen();
uint8_t sendDiscoverToGateway();
uint8_t sendWakeupCommandToNode(uint32_t id, uint8_t checkAttentionFlag);
uint8_t isDiscoverPackage(uint8_t response_length, uint8_t* _packet);
void initRadioLink();

uint8_t broadcast_rx(uint8_t* rx_buffer);
void broadcast_tx(uint8_t* rx_buffer);

} // namespace
