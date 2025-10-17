#pragma once

#include <avr/io.h>
#include <stddef.h>
#include <util/delay.h>

namespace COMMAND_HANDLERS{

void commandEepromRead(uint8_t* commandPayload, uint8_t* responsePayload);
void commandEepromWrite(uint8_t* commandPayload, uint8_t* responsePayload);
void commandSetDeviceName(uint8_t* commandPayload, uint8_t* responsePayload);
void commandGetDeviceName(uint8_t* commandPayload, uint8_t* responsePayload);
void commandSetRadioRole(uint8_t* commandPayload, uint8_t* responsePayload);

} // namespace COMMAND_HANDLERS

