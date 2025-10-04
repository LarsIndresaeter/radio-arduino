#pragma once

#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <nrf24l01.hpp>
#include <quadencoder.hpp>
#include <radio_link.hpp>
#include <util/delay.h>

namespace SLEEP {
void rfNodeSleepAndPollForWakeup();
void powerSaveSleepMs(uint8_t delay_ms);
void powerDownRadioAndSleep(uint16_t delay);
} // namespace

