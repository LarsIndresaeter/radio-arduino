#pragma once

#include <aes.hpp>
#include <avr/interrupt.h>
#include <cmd/payloads.hxx>
#include <nrf24l01.hpp>
#include <protocol.hpp>
#include <radio_link.hpp>
#include <random.hpp>
#include <sleep.hpp>
#include <util/delay.h>

extern void commandSwitch(uint8_t* commandPayload, uint8_t* responsePayload, ComBusInterface* comBus);

namespace PARSER {
bool lastReceivedCommandWasEncrypted();
void sendMessage(Protocol protocol, ComBusInterface* comBus, uint8_t* payload);
void parseInput(Protocol protocol, ComBusInterface* comBus);
void parseCommand(Protocol& protocol, ComBusInterface* comBus, uint8_t* commandPayload);
void setRequireTransportEncryption(uint8_t isRequired);

void setKeepAliveInterval(uint8_t interval);
uint16_t getCommandsParsedCounter();
} // namespace

