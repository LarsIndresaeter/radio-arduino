#pragma once

#include <openssl/aes.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <openssl/evp.h>

class Crypto {
public:
    class AesBlock {
    public:
        AesBlock() = default;

        AesBlock(uint8_t d[16])
        {
            for (int i = 0; i < 16; i++) {
                if (i >= 16) {
                    break;
                }
                m_data[i] = d[i];
            }
        }
        AesBlock(std::vector<uint8_t> d)
        {
            for (int i = 0; i < d.size(); i++) {
                if (i >= 16) {
                    break;
                }
                m_data[i] = d.at(i);
            }
        }
        std::string getDataAsString()
        {
            std::string retval;

            for (int i = 0; i < 16; i++) {
                retval += m_data[i];
            }
            return (retval);
        }
        uint8_t m_data[16] = {};
    };

    Crypto();
    ~Crypto();
    void setDecryptKey(AesBlock key);
    void setEncryptKey(AesBlock key);
    AesBlock encrypt(AesBlock plaintext);
    AesBlock decrypt(AesBlock ciphertext);
    std::vector<uint8_t> getVector(AesBlock block);

private:
    AesBlock encrypt_key = {};
    AesBlock decrypt_key = {};
    EVP_CIPHER_CTX *ctx;
    uint8_t iv[16] = {0};
};

