#pragma once

#include <stdint.h>
#include <nrf24l01.hpp>
#include <util/delay.h>

uint8_t sendDiscoverToGateway();
uint8_t wakeupCommand(uint8_t checkAttentionFlag);

uint8_t is_discover_package(uint8_t response_length, uint8_t* _packet);
