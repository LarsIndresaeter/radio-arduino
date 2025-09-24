#pragma once

#include <aes.hpp>
#include <protocol.hpp>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <nrf24l01.hpp>
#include <util/delay.h>
#include <cmd/payloads.hpp>
#include <random.hpp>

extern void parseCommand(
    Protocol& protocol, ComBusInterface* comBus, uint8_t* commandPayload);

void powerSaveSleepMs(uint8_t delay_ms);
void powerDownRadioAndSleep(uint16_t delay);
void rxNodeSleepAndPollForWakeup();
void sendMessage(Protocol protocol, ComBusInterface* comBus, uint8_t* payload);
void parseInput(Protocol protocol, ComBusInterface* comBus);
