#pragma once

#include <stdint.h>
#include <nrf24l01.hpp>
#include <util/delay.h>
#include <quadencoder.hpp>

uint8_t sendDiscoverToGateway();
uint8_t sendWakeupCommandToNode(uint8_t checkAttentionFlag);
uint8_t is_discover_package(uint8_t response_length, uint8_t* _packet);
void setNodeAddress(uint8_t address);

