#pragma once

#include <avr/io.h>
#include <stddef.h>
#include <util/delay.h>

namespace NRF24L01 {

void commandNrf24l01Init(uint8_t* commandPayload, uint8_t* responsePayload);
void commandNrf24l01Read(uint8_t* commandPayload, uint8_t* responsePayload);
void commandNrf24l01Write(uint8_t* commandPayload, uint8_t* responsePayload);

} // namespace NRF24L01
