#pragma once

#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <nrf24l01.hpp>
#include <util/delay.h>

void powerSaveSleepMs(uint8_t delay_ms);
void powerDownRadioAndSleep(uint16_t delay);
