#pragma once

#include <cryptoHandlerInterface.hpp>
#include <openssl/aes.h>
#include <openssl/evp.h>

class LinuxCryptoHandler : public CryptoHandlerInterface {
public:
    LinuxCryptoHandler();
    void encrypt(uint8_t length, uint8_t* buffer) override;
    void decrypt(uint8_t length, uint8_t* buffer) override;
    uint32_t checksum(uint8_t length, uint8_t* buffer) override;
    uint32_t mac(uint8_t length, uint8_t* buffer) override;
    uint32_t nonce() override;
    void setTransportKey(uint8_t tk[16]);
    void setMacKey(uint8_t mk[16]);

private:
    uint8_t m_tk[16];
    uint8_t m_mk[16];
    EVP_CIPHER_CTX* ctx;
    EVP_MD_CTX* mdctx;

    void encryptLastBlock(uint8_t length, uint8_t* buffer);
    void decryptLastBlock(uint8_t length, uint8_t* buffer);
};
