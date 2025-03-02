#pragma once

#include <cryptoHandlerInterface.hpp>
#include <stdint.h>

#define CRC32_POLY 0x04C11DB7 /* AUTODIN II, Ethernet, & FDDI */

namespace PROTOCOL {
namespace ENCRYPTED {
    constexpr uint8_t CHECKSUM_OFFSET
        = 4 + 0; // inner checksum. Verify correct decryption
    constexpr uint8_t MESSAGE_ID_OFFSET
        = 4 + 4; // should be incremented for each message to prevent playback
                 // attack
    constexpr uint8_t NONCE_OFFSET = 4 + 8;
    constexpr uint8_t RESERVED_OFFSET = 4 + 12;
    constexpr uint8_t PAYLOAD_OFFSET = 4 + 16;
    constexpr uint8_t HEADER_LENGTH = 4 + 16;
    constexpr uint8_t CRYPTO_OVERHEAD = 4 + 16;
    constexpr uint8_t MAC_LENGTH
        = 4; // outer message authentification code. Must pass before decryption
}

namespace HEADER {
    constexpr uint8_t SYNC0_OFFSET = 0;
    constexpr uint8_t SYNC1_OFFSET = 1;
    constexpr uint8_t VERSION_OFFSET = 2;
    constexpr uint8_t LENGTH_OFFSET = 3;
    constexpr uint8_t LENGTH = 4;
    constexpr uint8_t SYNC_PATTERN_BYTE_0 = 0xFE;
    constexpr uint8_t SYNC_PATTERN_BYTE_1 = 0xED;

    enum class VERSION
    {
        UNDEFINED = 0,
        BINARY_AND_TEXT = 1,
        ENCRYPTED_BINARY_AND_TEXT = 2,
        RADIO_BINARY_AND_TEXT = 3,
        RADIO_ENCRYPTED_BINARY_AND_TEXT = 4,
        JSON = 5
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
            for (crcTableVal = (uint32_t)crcTableIndex << 24, j = 8; j > 0;
                 --j) {
                crcTableVal = (crcTableVal & 0x80000000)
                    ? ((crcTableVal << 1) ^ CRC32_POLY)
                    : (crcTableVal << 1);
            }
            tmp = (tmp << 8) ^ crcTableVal;
        }
        *pCrc = tmp;
    }

    void createPacket(uint8_t length, uint8_t* payload, uint8_t* packet, uint8_t protocol_version) // add isRadioFlag
    {
        uint32_t crc = 0;

        packet[PROTOCOL::HEADER::SYNC0_OFFSET]
            = PROTOCOL::HEADER::SYNC_PATTERN_BYTE_0;
        packet[PROTOCOL::HEADER::SYNC1_OFFSET]
            = PROTOCOL::HEADER::SYNC_PATTERN_BYTE_1;
        packet[PROTOCOL::HEADER::VERSION_OFFSET]
            = protocol_version;
        packet[PROTOCOL::HEADER::LENGTH_OFFSET] = length;
        for (int i = 0; i < length; i++) {
            packet[PROTOCOL::PAYLOAD::PAYLOAD_OFFSET + i] = payload[i];
        }
        CRC32_calculate(
            (uint8_t*)&packet[PROTOCOL::PAYLOAD::PAYLOAD_OFFSET], length, &crc);
        packet[PROTOCOL::HEADER::LENGTH + length] = crc >> 24;
        packet[PROTOCOL::HEADER::LENGTH + length + 1] = crc >> 16;
        packet[PROTOCOL::HEADER::LENGTH + length + 2] = crc >> 8;
        packet[PROTOCOL::HEADER::LENGTH + length + 3] = crc;
    }

    void createEncryptedPacket(
        uint8_t length, uint8_t* payload, uint8_t* packet, uint8_t protocol_version)
    {
        uint32_t crc = 0;

        packet[PROTOCOL::HEADER::SYNC0_OFFSET]
            = PROTOCOL::HEADER::SYNC_PATTERN_BYTE_0;
        packet[PROTOCOL::HEADER::SYNC1_OFFSET]
            = PROTOCOL::HEADER::SYNC_PATTERN_BYTE_1;
        packet[PROTOCOL::HEADER::VERSION_OFFSET] = protocol_version;
        packet[PROTOCOL::HEADER::LENGTH_OFFSET] = length + PROTOCOL::ENCRYPTED::CRYPTO_OVERHEAD;

        // create header which act as initialization vector
        uint32_t nonce = m_cryptoHandler->nonce();
        packet[PROTOCOL::ENCRYPTED::NONCE_OFFSET] = nonce >> 24;
        packet[PROTOCOL::ENCRYPTED::NONCE_OFFSET + 1] = nonce >> 16;
        packet[PROTOCOL::ENCRYPTED::NONCE_OFFSET + 2] = nonce >> 8;
        packet[PROTOCOL::ENCRYPTED::NONCE_OFFSET + 3] = nonce;

        packet[PROTOCOL::ENCRYPTED::RESERVED_OFFSET] = 0;
        packet[PROTOCOL::ENCRYPTED::RESERVED_OFFSET + 1] = 0;
        packet[PROTOCOL::ENCRYPTED::RESERVED_OFFSET + 2] = 0;
        packet[PROTOCOL::ENCRYPTED::RESERVED_OFFSET + 3] = 0;

        packet[PROTOCOL::ENCRYPTED::MESSAGE_ID_OFFSET] = m_lastMessageId >> 24;
        packet[PROTOCOL::ENCRYPTED::MESSAGE_ID_OFFSET + 1]
            = m_lastMessageId >> 16;
        packet[PROTOCOL::ENCRYPTED::MESSAGE_ID_OFFSET + 2]
            = m_lastMessageId >> 8;
        packet[PROTOCOL::ENCRYPTED::MESSAGE_ID_OFFSET + 3] = m_lastMessageId;
        m_lastMessageId++;

        // copy payload
        for (int i = 0; i < length; i++) {
            packet[PROTOCOL::ENCRYPTED::PAYLOAD_OFFSET + i] = payload[i];
        }

        // inner checksum of nonce, message_id and message
        uint32_t checksum = m_cryptoHandler->checksum(length + 12, &packet[8]);
        packet[PROTOCOL::ENCRYPTED::CHECKSUM_OFFSET] = checksum >> 24;
        packet[PROTOCOL::ENCRYPTED::CHECKSUM_OFFSET + 1] = checksum >> 16;
        packet[PROTOCOL::ENCRYPTED::CHECKSUM_OFFSET + 2] = checksum >> 8;
        packet[PROTOCOL::ENCRYPTED::CHECKSUM_OFFSET + 3] = checksum;

        // encrypt inner checksum, nonce, message_id and message
        m_cryptoHandler->encrypt(
            PROTOCOL::ENCRYPTED::CRYPTO_OVERHEAD - 4 + length, &packet[4]);

        // add message authentification code
        uint32_t mac = m_cryptoHandler->mac(length + 16, &packet[4]);
        packet[PROTOCOL::ENCRYPTED::CRYPTO_OVERHEAD + length] = mac >> 24;
        packet[PROTOCOL::ENCRYPTED::CRYPTO_OVERHEAD + length + 1] = mac >> 16;
        packet[PROTOCOL::ENCRYPTED::CRYPTO_OVERHEAD + length + 2] = mac >> 8;
        packet[PROTOCOL::ENCRYPTED::CRYPTO_OVERHEAD + length + 3] = mac;

        CRC32_calculate(
            (uint8_t*)&packet[PROTOCOL::PAYLOAD::PAYLOAD_OFFSET],
            length + PROTOCOL::ENCRYPTED::CRYPTO_OVERHEAD,
            &crc);
        packet[PROTOCOL::HEADER::LENGTH + PROTOCOL::ENCRYPTED::CRYPTO_OVERHEAD + length] = crc >> 24;
        packet[PROTOCOL::HEADER::LENGTH + PROTOCOL::ENCRYPTED::CRYPTO_OVERHEAD + length + 1] = crc >> 16;
        packet[PROTOCOL::HEADER::LENGTH + PROTOCOL::ENCRYPTED::CRYPTO_OVERHEAD + length + 2] = crc >> 8;
        packet[PROTOCOL::HEADER::LENGTH + PROTOCOL::ENCRYPTED::CRYPTO_OVERHEAD + length + 3] = crc;
    }

    uint8_t decryptPayload(uint8_t length, uint8_t* payload)
    {
        // validate mac
        uint32_t mac_calculated = m_cryptoHandler->mac(length - 4, payload);
        uint32_t mac_package;
        mac_package = ((uint32_t)payload[length - 4]) << 24;
        mac_package |= ((uint32_t)payload[length - 3]) << 16;
        mac_package |= ((uint32_t)payload[length - 2]) << 8;
        mac_package |= ((uint32_t)payload[length - 1]);

        if (mac_calculated != mac_package) {
            return 0;
        }

        // decrypt payload
        m_cryptoHandler->decrypt(length - 4, &payload[0]);

        // validate checksum
        uint32_t checksum_calculated
            = m_cryptoHandler->checksum(length - 8, &payload[4]);
        uint32_t checksum_package;
        checksum_package = ((uint32_t)payload[0]) << 24;
        checksum_package |= ((uint32_t)payload[1]) << 16;
        checksum_package |= ((uint32_t)payload[2]) << 8;
        checksum_package |= ((uint32_t)payload[3]);

        uint32_t message_id;
        message_id = ((uint32_t)payload[4]) << 24;
        message_id |= ((uint32_t)payload[5]) << 16;
        message_id |= ((uint32_t)payload[6]) << 8;
        message_id |= ((uint32_t)payload[7]);

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
            payload[2] = 1;               // protocol changed to not encrypted
            payload[3] = payload[3] - PROTOCOL::ENCRYPTED::CRYPTO_OVERHEAD; // stripped away crypto overhead

            return payload[3];
        }
    }

protected:
    CryptoHandlerInterface* m_cryptoHandler;
    uint32_t m_lastMessageId = 0;
};

