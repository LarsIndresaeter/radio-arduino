#include <arduinoCryptoHandler.hpp>

ArduinoCryptoHandler::ArduinoCryptoHandler(uint8_t* tk)
{
    setMacKey(tk);
    setTransportKey(tk);
}

void ArduinoCryptoHandler::encryptLastBlock(uint8_t length, uint8_t* buffer)
{
    uint8_t aes_iv[16] = { 0 };

    AES::Crypt(&buffer[length-16], &m_tk[0], &aes_iv[0]);
}

void ArduinoCryptoHandler::encrypt(uint8_t length, uint8_t* buffer)
{
    uint8_t aes_iv[16] = { 0 };

    AES::Sanitize();

    for(int i = 0; i<(length-16); i += 16)
    {
        AES::Crypt(&buffer[i], &m_tk[0], &aes_iv[0]);

        for(int j=0;j<16;j++){
            aes_iv[j] = buffer[i + j];
        }
        // ecb mode
    }

    if((length > 16) && (length % 16 != 0))
    {
        encryptLastBlock(length, buffer);
    }
}


void ArduinoCryptoHandler::decryptLastBlock(uint8_t length, uint8_t* buffer)
{
    uint8_t aes_iv[16] = { 0 };
    AES::Decrypt(&buffer[length-16], &m_tk[0], &aes_iv[0]);
}

void ArduinoCryptoHandler::decrypt(uint8_t length, uint8_t* buffer)
{
    uint8_t aes_iv[16] = { 0 };
    uint8_t tmp[16] = { 0 };

    AES::Sanitize();

    if((length > 16) && (length % 16 != 0))
    {
        decryptLastBlock(length, buffer);
    }

    for(int i = 0; i<(length-16); i += 16)
    {
        for(int j=0;j<16;j++){
            tmp[j] = buffer[i + j];
        }

        AES::Decrypt(&buffer[i], &m_tk[0], &aes_iv[0]);
        // ecb mode
        for(int j=0;j<16;j++){
            aes_iv[j] = tmp[j];
        }
    }
}

uint32_t ArduinoCryptoHandler::checksum(uint8_t length, uint8_t* buffer)
{
    uint8_t shasum[20];
    uint32_t retval = 0;

    SHA1Context sha;

    SHA1Reset(&sha);
    SHA1Input(&sha, m_mk, 16);
    SHA1Input(&sha, &buffer[0], length);
    SHA1Result(&sha, &shasum[0]);

    retval |= ((uint32_t)shasum[0]) << 24;
    retval |= ((uint32_t)shasum[1]) << 16;
    retval |= ((uint32_t)shasum[2]) << 8;
    retval |= ((uint32_t)shasum[3]) << 0;

    return retval;
}

uint32_t ArduinoCryptoHandler::mac(uint8_t length, uint8_t* buffer)
{
    uint8_t shasum[20];
    uint32_t retval = 0;

    SHA1Context sha;

    SHA1Reset(&sha);
    SHA1Input(&sha, m_mk, 16);
    SHA1Input(&sha, buffer, length);
    SHA1Input(&sha, m_mk, 16);
    SHA1Result(&sha, shasum);

    retval |= ((uint32_t)shasum[0]) << 24;
    retval |= ((uint32_t)shasum[1]) << 16;
    retval |= ((uint32_t)shasum[2]) << 8;
    retval |= ((uint32_t)shasum[3]) << 0;

    return retval;
}

uint32_t ArduinoCryptoHandler::nonce()
{
    uint32_t retval = 0;

    retval = ((uint32_t)AtmelAdc::getRandomByte()) << 24;
    retval |= ((uint32_t)AtmelAdc::getRandomByte()) << 16;
    retval |= ((uint32_t)AtmelAdc::getRandomByte()) << 8;
    retval |= ((uint32_t)AtmelAdc::getRandomByte());

    return retval;
}

void ArduinoCryptoHandler::setTransportKey(uint8_t tk[16])
{
    for (int i = 0; i < 16; i++) {
        m_tk[i] = tk[i];
    }
}
void ArduinoCryptoHandler::setMacKey(uint8_t mk[16])
{
    for (int i = 0; i < 16; i++) {
        m_mk[i] = mk[i];
    }
}
