#pragma once

#include <avr/io.h>
#include <stddef.h>

namespace COMMAND_HANDLERS {

void commandDebug(uint8_t* commandPayload, uint8_t* responsePayload);
void commandPing(uint8_t* commandPayload, uint8_t* responsePayload);
void commandGetVersion(uint8_t* commandPayload, uint8_t* responsePayload);
void commandGetStatistics(uint8_t* commandPayload, uint8_t* responsePayload);

} // namespace COMMAND_HANDLERS

