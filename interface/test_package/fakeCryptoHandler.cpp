#include "fakeCryptoHandler.hpp"

FakeCryptoHandler::FakeCryptoHandler() {}

void FakeCryptoHandler::encrypt(uint8_t length, uint8_t* buffer)
{
    // for the unit test we don't bother to encrypt or decrypt
}

void FakeCryptoHandler::decrypt(uint8_t length, uint8_t* buffer)
{
    // for the unit test we don't bother to encrypt or decrypt
}

uint32_t FakeCryptoHandler::checksum(uint8_t length, uint8_t* buffer)
{
    return 0x33333333;
}

uint32_t FakeCryptoHandler::mac(uint8_t length, uint8_t* buffer)
{
    return 0x01234567;
}

uint32_t FakeCryptoHandler::nonce() { return 0x11111111; }

