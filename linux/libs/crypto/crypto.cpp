#include <crypto.hpp>

#include <openssl/aes.h>

Crypto::Crypto() { ctx = EVP_CIPHER_CTX_new(); }

Crypto::~Crypto()
{
    // EVP_CIPHER_CTX_free(ctx);
}

void Crypto::setEncryptKey(AesBlock key) { encrypt_key = key; }

void Crypto::setDecryptKey(AesBlock key) { decrypt_key = key; }

Crypto::AesBlock Crypto::encrypt(Crypto::AesBlock plaintext)
{
    AesBlock ciphertext {};
    int len;

    EVP_CIPHER_CTX_init(ctx);
    EVP_CIPHER_CTX_set_padding(ctx, false);
    EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, &encrypt_key.m_data[0], NULL);
    EVP_EncryptUpdate(ctx, &ciphertext.m_data[0], &len, &plaintext.m_data[0], 16);
    EVP_EncryptFinal(ctx, &ciphertext.m_data[0], &len);
    EVP_CIPHER_CTX_free(ctx);

    return (ciphertext);
}

Crypto::AesBlock Crypto::decrypt(Crypto::AesBlock ciphertext)
{
    AesBlock plaintext {};
    int len;

    EVP_CIPHER_CTX_init(ctx);
    EVP_CIPHER_CTX_set_padding(ctx, false);
    EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, &decrypt_key.m_data[0], NULL);
    EVP_DecryptUpdate(ctx, &plaintext.m_data[0], &len, &ciphertext.m_data[0], 16);
    EVP_DecryptFinal(ctx, &plaintext.m_data[0], &len);
    EVP_CIPHER_CTX_free(ctx);

    return (plaintext);
}

std::vector<uint8_t> Crypto::getVector(Crypto::AesBlock block)
{
    std::vector<uint8_t> retval {};

    for (int i = 0; i < 16; i++) {
        retval.push_back(block.m_data[i]);
    }

    return (retval);
}
