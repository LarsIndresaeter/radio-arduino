#pragma once

#include <adc.hpp>
#include <aes.hpp>
#include <cryptoHandlerInterface.hpp>
#include <sha1.hpp>

class ArduinoCryptoHandler : public CryptoHandlerInterface {
public:
    ArduinoCryptoHandler(uint8_t* tk);
    virtual void encrypt(uint8_t length, uint8_t* buffer);
    virtual void decrypt(uint8_t length, uint8_t* buffer);
    virtual uint32_t checksum(uint8_t length, uint8_t* buffer);
    virtual uint32_t mac(uint8_t length, uint8_t* buffer);
    virtual uint32_t nonce();
    virtual void setTransportKey(uint8_t tk[16]);
    virtual void setMacKey(uint8_t mk[16]);

private:
    uint8_t m_tk[16];
    uint8_t m_mk[16];

    virtual void encryptLastBlock(uint8_t length, uint8_t* buffer);
    virtual void decryptLastBlock(uint8_t length, uint8_t* buffer);
};

