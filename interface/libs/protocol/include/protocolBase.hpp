#pragma once

#include <cryptoHandlerInterface.hpp>
#include <stdint.h>

#define CRC32_POLY 0x04C11DB7 /* AUTODIN II, Ethernet, & FDDI */

namespace PROTOCOL {
namespace ENCRYPTED {
    constexpr uint8_t CHECKSUM_OFFSET = 4 + 0;   // inner checksum. Verify correct decryption
    constexpr uint8_t MESSAGE_ID_OFFSET = 4 + 4; // should be incremented for each message to prevent playback
                                                 // attack
    constexpr uint8_t NONCE_OFFSET = 4 + 8;
    constexpr uint8_t RESERVED_OFFSET = 4 + 12;
    constexpr uint8_t PAYLOAD_OFFSET = 4 + 16;
    constexpr uint8_t HEADER_LENGTH = 4 + 16;
    constexpr uint8_t CRYPTO_OVERHEAD = 4 + 16;
    constexpr uint8_t MAC_LENGTH = 4; // outer message authentification code. Must pass before decryption
}

namespace HEADER {
    constexpr uint8_t SYNC0_OFFSET = 0;
    constexpr uint8_t SYNC1_OFFSET = 1;
    constexpr uint8_t VERSION_OFFSET = 2;
    constexpr uint8_t LENGTH_OFFSET = 3;
    constexpr uint8_t LENGTH = 4;
    constexpr uint8_t SYNC_PATTERN_BYTE_0 = 0xFE;
    constexpr uint8_t SYNC_PATTERN_BYTE_1 = 0xED;

    // version[2]=radio
    // version[1]=encrypted
    // version[0]=1 (0 is undefined)
    enum class VERSION
    {
        UNDEFINED = 0,
        GATEWAY= 1,            // 0b0000 0001
        GATEWAY_ENCRYPTED = 3, // 0b0000 0011
        NODE= 5,               // 0b0000 0101
        NODE_ENCRYPTED = 7,    // 0b0000 0111
    };
}

namespace PAYLOAD {
    constexpr uint8_t PAYLOAD_OFFSET = 4;
    constexpr uint8_t OI_OFFSET = 4;
    constexpr uint8_t OL_OFFSET = 5;
}

namespace CHECKSUM {
    constexpr uint8_t LENGTH = 4;
}
}

class ProtocolBase {
public:
    ProtocolBase(CryptoHandlerInterface* cryptoHandler)
        : m_cryptoHandler(cryptoHandler) {};

    void CRC32_calculate(uint8_t* buf, uint16_t length, uint32_t* pCrc)
    {
        uint8_t* p;
        uint32_t tmp;

        tmp = *pCrc;

        for (p = &buf[0]; length > 0; ++p, --length) {
            uint32_t crcTableVal;
            uint8_t crcTableIndex = (uint8_t)(tmp >> 24) ^ *p;
            uint8_t j;
            for (crcTableVal = (uint32_t)crcTableIndex << 24, j = 8; j > 0; --j) {
                crcTableVal = (crcTableVal & 0x80000000) ? ((crcTableVal << 1) ^ CRC32_POLY) : (crcTableVal << 1);
            }
            tmp = (tmp << 8) ^ crcTableVal;
        }
        *pCrc = tmp;
    }

    uint32_t readUint32FromBuffer(uint8_t* buffer)
    {
        uint32_t retval = 0;

        retval = (uint32_t)buffer[0] << 24;
        retval |= (uint32_t)buffer[1] << 16;
        retval |= (uint32_t)buffer[2] << 8;
        retval |= (uint32_t)buffer[3];

        return (retval);
    }

    void writeUint32ToBuffer(uint8_t* buffer, uint32_t value)
    {
        buffer[0] = (uint8_t)(value >> 24);
        buffer[1] = (uint8_t)(value >> 16);
        buffer[2] = (uint8_t)(value >> 8);
        buffer[3] = (uint8_t)(value);
    }

    void createPacket(uint8_t length, uint8_t* payload, uint8_t* packet, uint8_t protocol_version) // add isRadioFlag
    {
        uint32_t crc = 0;

        packet[PROTOCOL::HEADER::SYNC0_OFFSET] = PROTOCOL::HEADER::SYNC_PATTERN_BYTE_0;
        packet[PROTOCOL::HEADER::SYNC1_OFFSET] = PROTOCOL::HEADER::SYNC_PATTERN_BYTE_1;
        packet[PROTOCOL::HEADER::VERSION_OFFSET] = protocol_version;
        packet[PROTOCOL::HEADER::LENGTH_OFFSET] = length;
        for (int i = 0; i < length; i++) {
            packet[PROTOCOL::PAYLOAD::PAYLOAD_OFFSET + i] = payload[i];
        }
        CRC32_calculate((uint8_t*)&packet[PROTOCOL::PAYLOAD::PAYLOAD_OFFSET], length, &crc);
        writeUint32ToBuffer(&packet[PROTOCOL::HEADER::LENGTH + length], crc);
    }

    void createEncryptedPacket(uint8_t length, uint8_t* payload, uint8_t* packet, uint8_t protocol_version)
    {
        uint32_t crc = 0;

        packet[PROTOCOL::HEADER::SYNC0_OFFSET] = PROTOCOL::HEADER::SYNC_PATTERN_BYTE_0;
        packet[PROTOCOL::HEADER::SYNC1_OFFSET] = PROTOCOL::HEADER::SYNC_PATTERN_BYTE_1;
        packet[PROTOCOL::HEADER::VERSION_OFFSET] = protocol_version;
        packet[PROTOCOL::HEADER::LENGTH_OFFSET] = length + PROTOCOL::ENCRYPTED::CRYPTO_OVERHEAD;

        // create header which act as initialization vector
        writeUint32ToBuffer(&packet[PROTOCOL::ENCRYPTED::MESSAGE_ID_OFFSET], m_lastMessageId);
        m_lastMessageId++;
        uint32_t nonce = m_cryptoHandler->nonce();
        writeUint32ToBuffer(&packet[PROTOCOL::ENCRYPTED::NONCE_OFFSET], nonce);
        writeUint32ToBuffer(&packet[PROTOCOL::ENCRYPTED::RESERVED_OFFSET], 0);

        // copy payload
        for (int i = 0; i < length; i++) {
            packet[PROTOCOL::ENCRYPTED::PAYLOAD_OFFSET + i] = payload[i];
        }

        // inner checksum of nonce, message_id and message
        uint32_t checksum = m_cryptoHandler->checksum(length + 12, &packet[8]);
        writeUint32ToBuffer(&packet[PROTOCOL::ENCRYPTED::CHECKSUM_OFFSET], checksum);

        // encrypt inner checksum, nonce, message_id and message
        m_cryptoHandler->encrypt(PROTOCOL::ENCRYPTED::CRYPTO_OVERHEAD - 4 + length, &packet[4]);

        // add message authentification code
        uint32_t mac = m_cryptoHandler->mac(length + 16, &packet[PROTOCOL::ENCRYPTED::CHECKSUM_OFFSET]);
        writeUint32ToBuffer(&packet[PROTOCOL::ENCRYPTED::CRYPTO_OVERHEAD + length], mac);

        CRC32_calculate(
            (uint8_t*)&packet[PROTOCOL::PAYLOAD::PAYLOAD_OFFSET], length + PROTOCOL::ENCRYPTED::CRYPTO_OVERHEAD, &crc);
        writeUint32ToBuffer(&packet[PROTOCOL::HEADER::LENGTH + PROTOCOL::ENCRYPTED::CRYPTO_OVERHEAD + length], crc);
    }

    uint8_t decryptPayload(uint8_t length, uint8_t* payload)
    {
        // validate mac
        uint32_t mac_calculated = m_cryptoHandler->mac(length - 4, payload);
        uint32_t mac_package = readUint32FromBuffer(&payload[length - 4]);

        if (mac_calculated != mac_package) {
            return 0;
        }

        // decrypt payload
        m_cryptoHandler->decrypt(length - 4, &payload[0]);

        // validate checksum
        uint32_t checksum_calculated = m_cryptoHandler->checksum(length - 8, &payload[4]);
        uint32_t checksum_package = readUint32FromBuffer(&payload[0]);
        uint32_t message_id = readUint32FromBuffer(&payload[4]);

        if (message_id > m_lastMessageId) {
            m_lastMessageId = message_id;
        }

        if (checksum_calculated != checksum_package) {
            return 0;
        }

        // copy payload
        for (uint8_t i = 0; i < length - PROTOCOL::ENCRYPTED::CRYPTO_OVERHEAD; i++) {
            payload[i] = payload[16 + i];
        }

        return length - PROTOCOL::ENCRYPTED::CRYPTO_OVERHEAD;
    }

    uint8_t decryptPackage(uint8_t* payload)
    {
        if (0 == decryptPayload(payload[3], &payload[4])) {
            return 0;
        }
        else {
            // update package header
            payload[2] = 1;                                                 // protocol changed to not encrypted
            payload[3] = payload[3] - PROTOCOL::ENCRYPTED::CRYPTO_OVERHEAD; // stripped away crypto overhead

            return payload[3];
        }
    }

protected:
    CryptoHandlerInterface* m_cryptoHandler;
    uint32_t m_lastMessageId = 0;
};
