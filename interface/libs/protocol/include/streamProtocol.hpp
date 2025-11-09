#pragma once

#include <protocolBase.hpp>

class StreamProtocol : public ProtocolBase {
public:
    StreamProtocol(CryptoHandlerInterface* cryptoHandler)
        : ProtocolBase(cryptoHandler) {};

    uint8_t searchForMessage(uint8_t* payload, uint8_t* protocolVersionLastReceivedMessage)
    {
        uint8_t length = 0;
        uint8_t protocolVersion = 0;
        uint8_t index = 0;
        uint8_t c;
        bool commandReceived = false;
        uint32_t checksumReceived = 0;
        uint32_t checksumCalculated = 0;

        // first sync byte implicitly received
        while (true) {
            if (hasData()) {
                index++;
                c = getChar();
                if (index == 1) {
                    if (c != PROTOCOL::HEADER::SYNC_PATTERN_BYTE_1) {
                        return 0;
                    }
                }

                else if (index == PROTOCOL::HEADER::VERSION_OFFSET) {
                    protocolVersion = c;
                    if (!((protocolVersion == static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::GATEWAY))
                          || (protocolVersion
                              == static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::GATEWAY_ENCRYPTED))
                          || (protocolVersion == static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::NODE))
                          || (protocolVersion
                              == static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::NODE_ENCRYPTED)))) {
                        return 0;
                    }
                }
                else if (index == PROTOCOL::HEADER::LENGTH_OFFSET) {
                    length = c;
                }
                else if ((index >= PROTOCOL::HEADER::LENGTH) && (index < PROTOCOL::HEADER::LENGTH + length)) {
                    payload[index - PROTOCOL::PAYLOAD::PAYLOAD_OFFSET] = c;
                }
                else if (index == (PROTOCOL::HEADER::LENGTH + length)) {
                    checksumReceived = (((uint32_t)c) << 24);
                }
                else if (index == (PROTOCOL::HEADER::LENGTH + length + 1)) {
                    checksumReceived |= (((uint32_t)c) << 16);
                }
                else if (index == (PROTOCOL::HEADER::LENGTH + length + 2)) {
                    checksumReceived |= (((uint32_t)c) << 8);
                }
                else if (index == (PROTOCOL::HEADER::LENGTH + length + 3)) {
                    checksumReceived |= c;
                    commandReceived = true;
                }
                else {
                    return 0;
                }

                if (commandReceived) {
                    CRC32_calculate(&payload[0], length, &checksumCalculated);

                    if (checksumReceived == checksumCalculated) {
                        if (protocolVersion
                                == static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::GATEWAY_ENCRYPTED)
                            || protocolVersion
                                == static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::NODE_ENCRYPTED)) {
                            length = decryptPayload(length, payload);
                        }

                        *protocolVersionLastReceivedMessage = protocolVersion;

                        return length;
                    }
                }
            }
        }

        return 0;
    }

    virtual char getChar() { return 0; }     // TODO: this should probably be pure virtual
    virtual bool hasData() { return false; } // TODO: this should probably be pure virtual
};
