#include <arduinoCryptoHandler.hpp>

ArduinoCryptoHandler::ArduinoCryptoHandler(
    aes& p_aes)
    : m_aes(p_aes)
{
    uint8_t tmp[16] = { 0 };
    tmp[0]='s';
    tmp[1]='e';
    tmp[2]='c';
    tmp[3]='r';
    tmp[4]='e';
    tmp[5]='t';
    setMacKey(tmp);
    setTransportKey(tmp);
}

void ArduinoCryptoHandler::encryptLastBlock(uint8_t length, uint8_t* buffer)
{
    uint8_t aes_iv[16] = { 0 };

    m_aes.Crypt(&buffer[length-16], &m_tk[0], &aes_iv[0]);
}

void ArduinoCryptoHandler::encrypt(uint8_t length, uint8_t* buffer)
{
    uint8_t aes_iv[16] = { 0 };

    for(int i = 0; i<(length-16); i += 16)
    {
        m_aes.Crypt(&buffer[i], &m_tk[0], &aes_iv[0]);

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
    m_aes.Decrypt(&buffer[length-16], &m_tk[0], &aes_iv[0]);
}

void ArduinoCryptoHandler::decrypt(uint8_t length, uint8_t* buffer)
{
    uint8_t aes_iv[16] = { 0 };
    uint8_t tmp[16] = { 0 };

    if((length > 16) && (length % 16 != 0))
    {
        decryptLastBlock(length, buffer);
    }

    for(int i = 0; i<(length-16); i += 16)
    {
        for(int j=0;j<16;j++){
            tmp[j] = buffer[i + j];
        }

        m_aes.Decrypt(&buffer[i], &m_tk[0], &aes_iv[0]);
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

    SHA1Reset(&m_sha);
    SHA1Input(&m_sha, m_mk, 16);
    SHA1Input(&m_sha, buffer, length);
    SHA1Result(&m_sha, shasum);

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

    SHA1Reset(&m_sha);
    SHA1Input(&m_sha, m_mk, 16);
    SHA1Input(&m_sha, buffer, length);
    SHA1Input(&m_sha, m_mk, 16);
    SHA1Result(&m_sha, shasum);

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
