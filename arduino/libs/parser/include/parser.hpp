#pragma once

#include <aes.hpp>
#include <protocol.hpp>
#include <avr/interrupt.h>
#include <nrf24l01.hpp>
#include <util/delay.h>
#include <cmd/payloads.hpp>
#include <random.hpp>
#include <sleep.hpp>

extern void parseCommand(
    Protocol& protocol, ComBusInterface* comBus, uint8_t* commandPayload);

void rxNodeSleepAndPollForWakeup();
void sendMessage(Protocol protocol, ComBusInterface* comBus, uint8_t* payload);
void parseInput(Protocol protocol, ComBusInterface* comBus);
