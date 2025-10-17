#pragma once

#include <avr/io.h>
#include <stddef.h>
#include <util/delay.h>

namespace COMMAND_HANDLERS{

void commandNrf24l01Init(uint8_t* commandPayload, uint8_t* responsePayload);
void commandNrf24l01Read(uint8_t* commandPayload, uint8_t* responsePayload);
void commandNrf24l01Write(uint8_t* commandPayload, uint8_t* responsePayload);

} // namespace COMMAND_HANDLERS

