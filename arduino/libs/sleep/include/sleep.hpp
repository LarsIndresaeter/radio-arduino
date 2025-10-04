#pragma once

#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <nrf24l01.hpp>
#include <radio_link.hpp>
#include <util/delay.h>

extern uint8_t rf_link_wakeup_command[32];
extern uint8_t rf_link_discover_package[32];
extern uint8_t attention_flag;

void rxNodeSleepAndPollForWakeup();
void powerSaveSleepMs(uint8_t delay_ms);
void powerDownRadioAndSleep(uint16_t delay);
