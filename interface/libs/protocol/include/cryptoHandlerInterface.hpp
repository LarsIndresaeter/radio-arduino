#pragma once

#include <stdint.h>

class CryptoHandlerInterface {
public:
    virtual void encrypt(uint8_t length, uint8_t* buffer) = 0;
    virtual void decrypt(uint8_t length, uint8_t* buffer) = 0;
    virtual uint32_t checksum(uint8_t length, uint8_t* buffer) = 0;
    virtual uint32_t mac(uint8_t length, uint8_t* buffer) = 0;
    virtual uint32_t nonce() = 0;
};
