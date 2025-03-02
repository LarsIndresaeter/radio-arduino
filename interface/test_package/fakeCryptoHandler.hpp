#pragma once

#include <cryptoHandlerInterface.hpp>

class FakeCryptoHandler : public CryptoHandlerInterface {
public:
    FakeCryptoHandler();
    virtual void encrypt(uint8_t length, uint8_t* buffer);
    virtual void decrypt(uint8_t length, uint8_t* buffer);
    virtual uint32_t checksum(uint8_t length, uint8_t* buffer);
    virtual uint32_t mac(uint8_t length, uint8_t* buffer);
    virtual uint32_t nonce();
};
