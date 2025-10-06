#include <linuxCryptoHandler.hpp>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/sha.h>

LinuxCryptoHandler::LinuxCryptoHandler()
{
    uint8_t tmp[16] = { 0 };
    tmp[0] = 't';
    tmp[1] = 'r';
    tmp[2] = 'a';
    tmp[3] = 'n';
    tmp[4] = 's';
    tmp[5] = 'p';
    tmp[6] = 'o';
    tmp[7] = 'r';
    tmp[8] = 't';
    tmp[9] = '-';
    tmp[10] = 'k';
    tmp[11] = 'e';
    tmp[12] = 'y';
    setMacKey(tmp);
    setTransportKey(tmp);
}

void LinuxCryptoHandler::encryptLastBlock(uint8_t length, uint8_t* buffer)
{
    int len;
    uint8_t aes_iv[16] = { 0 };
    ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(ctx);
    EVP_CIPHER_CTX_set_padding(ctx, false);
    EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, m_tk, &aes_iv[0]);

    EVP_EncryptUpdate(ctx, &buffer[length-16], &len, &buffer[length-16], 16);

    EVP_EncryptFinal(ctx, &buffer[length - 16], &len);
    EVP_CIPHER_CTX_free(ctx);
}

void LinuxCryptoHandler::encrypt(uint8_t length, uint8_t* buffer)
{
    int len;
    uint8_t aes_iv[16] = { 0 };
    ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(ctx);
    EVP_CIPHER_CTX_set_padding(ctx, false);
    EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, m_tk, &aes_iv[0]);

    for (uint8_t i = 0; i < (length - 16); i += 16) {
        EVP_EncryptUpdate(ctx, &buffer[i], &len, &buffer[i], 16);
    }

    EVP_EncryptFinal(ctx, &buffer[length - 16], &len);
    EVP_CIPHER_CTX_free(ctx);

    if((length > 16) && (length % 16 != 0))
    {
        encryptLastBlock(length, buffer);
    }
}

void LinuxCryptoHandler::decryptLastBlock(uint8_t length, uint8_t* buffer)
{
    int len;
    uint8_t aes_iv[16] = { 0 };
    ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(ctx);
    EVP_CIPHER_CTX_set_padding(ctx, false);
    EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, m_tk, &aes_iv[0]);

    EVP_DecryptUpdate(ctx, &buffer[length-16], &len, &buffer[length-16], 16);

    EVP_DecryptFinal(ctx, &buffer[length - 16], &len);
    EVP_CIPHER_CTX_free(ctx);
}

void LinuxCryptoHandler::decrypt(uint8_t length, uint8_t* buffer)
{
    int len;
    uint8_t aes_iv[16] = { 0 };

    if((length > 16) && (length % 16 != 0))
    {
        decryptLastBlock(length, buffer);
    }

    ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(ctx);
    EVP_CIPHER_CTX_set_padding(ctx, false);
    EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, m_tk, &aes_iv[0]);

    for (uint8_t i = 0; i < (length - 16); i += 16) {
        EVP_DecryptUpdate(ctx, &buffer[i], &len, &buffer[i], 16);
    }

    EVP_DecryptFinal(ctx, &buffer[length - 16], &len);
    EVP_CIPHER_CTX_free(ctx);
}

uint32_t LinuxCryptoHandler::checksum(uint8_t length, uint8_t* buffer)
{
    uint32_t retval = 0;
    uint8_t shasum[20];
    unsigned int len = 0;

    mdctx = EVP_MD_CTX_create();
    EVP_DigestInit_ex(mdctx, EVP_sha1(), NULL);
    EVP_DigestUpdate(mdctx, m_mk, 16);
    EVP_DigestUpdate(mdctx, buffer, length);
    EVP_DigestFinal(mdctx, shasum, &len);
    EVP_MD_CTX_destroy(mdctx);

    retval |= ((uint32_t)shasum[0]) << 24;
    retval |= ((uint32_t)shasum[1]) << 16;
    retval |= ((uint32_t)shasum[2]) << 8;
    retval |= ((uint32_t)shasum[3]) << 0;

    return retval;
}

uint32_t LinuxCryptoHandler::mac(uint8_t length, uint8_t* buffer)
{
    uint32_t retval = 0;
    uint8_t shasum[20];
    unsigned int len = 0;

    mdctx = EVP_MD_CTX_create();
    EVP_DigestInit_ex(mdctx, EVP_sha1(), NULL);
    EVP_DigestUpdate(mdctx, m_mk, 16);
    EVP_DigestUpdate(mdctx, buffer, length);
    EVP_DigestUpdate(mdctx, m_mk, 16);
    EVP_DigestFinal(mdctx, shasum, &len);
    EVP_MD_CTX_destroy(mdctx);

    retval |= ((uint32_t)shasum[0]) << 24;
    retval |= ((uint32_t)shasum[1]) << 16;
    retval |= ((uint32_t)shasum[2]) << 8;
    retval |= ((uint32_t)shasum[3]) << 0;

    return retval;
}

uint32_t LinuxCryptoHandler::nonce()
{
    uint8_t buffer[128];
    uint32_t retval = 0;

    int rc = RAND_bytes(buffer, sizeof(buffer));
    // unsigned long err = ERR_get_error();

    retval |= ((uint32_t)buffer[0]) << 24;
    retval |= ((uint32_t)buffer[1]) << 16;
    retval |= ((uint32_t)buffer[2]) << 8;
    retval |= ((uint32_t)buffer[3]) << 0;

    return retval;
}

void LinuxCryptoHandler::setTransportKey(uint8_t tk[16])
{
    for (int i = 0; i < 16; i++) {
        m_tk[i] = tk[i];
    }
}
void LinuxCryptoHandler::setMacKey(uint8_t mk[16])
{
    for (int i = 0; i < 16; i++) {
        m_mk[i] = mk[i];
    }
}
