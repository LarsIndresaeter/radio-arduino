#pragma once

#include <aes.hpp>
#include <avr/interrupt.h>
#include <cmd/payloads.hpp>
#include <nrf24l01.hpp>
#include <radio_link.hpp>
#include <protocol.hpp>
#include <random.hpp>
#include <sleep.hpp>
#include <util/delay.h>

extern uint8_t attention_flag;
extern uint8_t protocolVersionLastReceivedMessage;

extern Random random;

extern uint16_t commandsParsed;

extern uint32_t rf_tx;
extern uint32_t rf_rx;

extern void commandSwitch(uint8_t* commandPayload, uint8_t* responsePayload, ComBusInterface* comBus);

void sendMessage(Protocol protocol, ComBusInterface* comBus, uint8_t* payload);
void parseInput(Protocol protocol, ComBusInterface* comBus);
void parseCommand(Protocol& protocol, ComBusInterface* comBus, uint8_t* commandPayload);

void setKeepAliveInterval(uint8_t interval);

