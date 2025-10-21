#pragma once

#include <cmd/payloads.hxx>
#include <protocolBase.hpp>
#include <string>
#include <vector>

class BufferProtocol : public ProtocolBase {
public:
    BufferProtocol(CryptoHandlerInterface* cryptoHandler)
        : ProtocolBase(cryptoHandler) {};

    std::vector<uint8_t> parseBinary(std::vector<uint8_t>& inputBuffer)
    {
        std::vector<uint8_t> retval;

        if (inputBuffer.size() < PROTOCOL::HEADER::LENGTH) {
            // need at least both sync bytes and length. Wait for more data
            return retval;
        }

        if (inputBuffer.at(PROTOCOL::HEADER::SYNC1_OFFSET) != PROTOCOL::HEADER::SYNC_PATTERN_BYTE_1) {
            // sync pattern mismatch. pop first sync byte and abort
            inputBuffer.erase(inputBuffer.begin());
            return retval;
        }

        uint8_t protocolVersion = inputBuffer.at(PROTOCOL::HEADER::VERSION_OFFSET);
        uint8_t length = inputBuffer.at(PROTOCOL::HEADER::LENGTH_OFFSET);

        if (inputBuffer.size() < (PROTOCOL::HEADER::LENGTH + length + PROTOCOL::CHECKSUM::LENGTH)) {
            // wait for buffer to fill up with the rest of the response
            return retval;
        }

        uint32_t checksumReceived = inputBuffer.at(PROTOCOL::HEADER::LENGTH + length) << 24;
        checksumReceived |= inputBuffer.at(PROTOCOL::HEADER::LENGTH + length + 1) << 16;
        checksumReceived |= inputBuffer.at(PROTOCOL::HEADER::LENGTH + length + 2) << 8;
        checksumReceived |= inputBuffer.at(PROTOCOL::HEADER::LENGTH + length + 3);

        uint32_t checksumCalculated = 0;
        uint8_t* p = inputBuffer.data();
        CRC32_calculate(&p[PROTOCOL::PAYLOAD::PAYLOAD_OFFSET], length, &checksumCalculated);

        if (checksumReceived == checksumCalculated) {
            for (int i = 0; i < (PROTOCOL::HEADER::LENGTH + length + PROTOCOL::CHECKSUM::LENGTH); i++) {
                retval.push_back(inputBuffer.at(i));
            }

            if (protocolVersion == static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::ENCRYPTED_BINARY_AND_TEXT)
                || protocolVersion
                    == static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::RADIO_ENCRYPTED_BINARY_AND_TEXT)) {
                uint8_t decrypt_size = decryptPackage(retval.data());
                retval.resize(decrypt_size + 8); // header + payload + crc
            }
        }

        // remove message from input buffer
        for (int i = 0; i < (PROTOCOL::HEADER::LENGTH + length + PROTOCOL::CHECKSUM::LENGTH); i++) {
            inputBuffer.erase(inputBuffer.begin());
        }

        return retval;
    }

    std::vector<uint8_t> createBinaryCommand(std::vector<uint8_t> payload, uint8_t protocol_version)
    {
        std::vector<uint8_t> command;

        if (protocol_version == static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::ENCRYPTED_BINARY_AND_TEXT)
            || protocol_version == static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::RADIO_ENCRYPTED_BINARY_AND_TEXT)) {
            command.resize(
                PROTOCOL::HEADER::LENGTH + PROTOCOL::CHECKSUM::LENGTH + payload.size()
                + PROTOCOL::ENCRYPTED::CRYPTO_OVERHEAD);

            createEncryptedPacket(payload.size(), payload.data(), command.data(), protocol_version);
        }
        else {
            command.resize(PROTOCOL::HEADER::LENGTH + PROTOCOL::CHECKSUM::LENGTH + payload.size());

            createPacket(payload.size(), payload.data(), command.data(), protocol_version);
        }

        return (command);
    }

    std::vector<uint8_t> searchForSync(std::vector<uint8_t>& data)
    {
        while (data.size() != 0) {
            if (data.at(0) == PROTOCOL::HEADER::SYNC_PATTERN_BYTE_0) {
                std::vector<uint8_t> packet = parseBinary(data);
                if (packet.size() > 0) {
                    return packet; // found packet
                }
                else {
                    return {}; // try again later
                }
            }
            else {
                // drop first byte and proceed with search for sync
                data.erase(data.begin());
            }
        }

        return {};
    }
};
