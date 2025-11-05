#pragma once

#include <avr/io.h>
#include <stddef.h>
#include <util/delay.h>

namespace PARSER {

void commandSetKey(uint8_t* commandPayload, uint8_t* responsePayload);
void commandKeepAlive(uint8_t* commandPayload, uint8_t* responsePayload);
void commandRequireTransportEncryption(uint8_t* commandPayload, uint8_t* responsePayload);
void commandUnlockSession(uint8_t* commandPayload, uint8_t* responsePayload);

} // namespace PARSER
