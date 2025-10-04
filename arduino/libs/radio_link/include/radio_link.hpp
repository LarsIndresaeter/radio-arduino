#pragma once

#include <stdint.h>
#include <nrf24l01.hpp>
#include <util/delay.h>

uint8_t sendDiscoverToGateway();
uint8_t wakeupCommand(uint8_t checkAttentionFlag);

