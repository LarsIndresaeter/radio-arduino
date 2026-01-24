#include <parser.hpp>

extern bool rx_mode_gateway;

namespace PARSER {
uint16_t commandsParsed = 0;

bool transportEncryptionIsRequired = true;

constexpr uint16_t inactive_time_before_rfnode_sleep_ms = 1000;
uint16_t count_down_ms_to_rfnode_sleep = inactive_time_before_rfnode_sleep_ms;

uint8_t protocolVersionLastReceivedMessage = static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::UNDEFINED);

uint16_t getCommandsParsedCounter() { return commandsParsed; }

void sendMessage(Protocol protocol, ComBusInterface* comBus, uint8_t* payload)
{
    uint8_t* packet = payload;
    uint8_t length = payload[1] + 2;

    if (protocolVersionLastReceivedMessage == static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::GATEWAY_ENCRYPTED)
        || ((rx_mode_gateway == false)
            && protocolVersionLastReceivedMessage == static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::NODE_ENCRYPTED))

    ) {
        protocol.createEncryptedPacket(length, payload, &packet[0], protocolVersionLastReceivedMessage);

        if (false == rx_mode_gateway) {
            NRF24L01_tx(
                &packet[0],
                PROTOCOL::HEADER::LENGTH + PROTOCOL::CHECKSUM::LENGTH + length + PROTOCOL::ENCRYPTED::CRYPTO_OVERHEAD);
        }
        else {
            comBus->writeBuffer(
                &packet[0],
                PROTOCOL::HEADER::LENGTH + PROTOCOL::CHECKSUM::LENGTH + length + PROTOCOL::ENCRYPTED::CRYPTO_OVERHEAD);
        }
    }
    else if (
        protocolVersionLastReceivedMessage == static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::GATEWAY)
        || ((rx_mode_gateway == false)
            && protocolVersionLastReceivedMessage == static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::NODE))) {
        protocol.createPacket(length, payload, &packet[0], protocolVersionLastReceivedMessage);

        if (false == rx_mode_gateway) {
            NRF24L01_tx(&packet[0], PROTOCOL::HEADER::LENGTH + PROTOCOL::CHECKSUM::LENGTH + length);
        }
        else {
            comBus->writeBuffer(&packet[0], PROTOCOL::HEADER::LENGTH + PROTOCOL::CHECKSUM::LENGTH + length);
        }
    }
}

void sendPayloadToRadioNode(Protocol protocol, uint8_t* payload, uint8_t length)
{
    uint8_t* packet = payload;
    uint8_t data_size = 0;

    if (protocolVersionLastReceivedMessage == static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::NODE)
        && rx_mode_gateway) {
        protocol.createPacket(length, payload, &packet[0], protocolVersionLastReceivedMessage);

        data_size = PROTOCOL::HEADER::LENGTH + PROTOCOL::CHECKSUM::LENGTH + length;

        // send command to rx_node and wait for response
        NRF24L01_tx(&packet[0], data_size);
    }
    else if (
        protocolVersionLastReceivedMessage == static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::NODE_ENCRYPTED)
        && rx_mode_gateway) {
        protocol.createEncryptedPacket(length, payload, &packet[0], protocolVersionLastReceivedMessage);

        data_size
            = PROTOCOL::HEADER::LENGTH + PROTOCOL::CHECKSUM::LENGTH + length + PROTOCOL::ENCRYPTED::CRYPTO_OVERHEAD;

        // send command to rx_node and wait for response
        NRF24L01_tx(&packet[0], data_size);
    }
}

void nodeIdleLoop()
{
    _delay_ms(1);
    if (count_down_ms_to_rfnode_sleep == 0) {
        SLEEP::rfNodeSleepAndPollForWakeup();
        count_down_ms_to_rfnode_sleep = inactive_time_before_rfnode_sleep_ms;
    }
    count_down_ms_to_rfnode_sleep--; // decrement time before going to sleep
}

void gatewayIdleLoop(ComBusInterface* comBus)
{
    // rx gateway reads response in idle loop
    uint8_t ack_packet[32];

    uint8_t response_length = NRF24L01_read_rx_payload(&ack_packet[0]);

    uint8_t is_wakeup_ack = RADIOLINK::isDiscoverPackage(response_length, &ack_packet[0]);

    if (is_wakeup_ack == 0) {
        comBus->writeBuffer(&ack_packet[0], response_length);
    }
}

void parseInput(Protocol protocol, ComBusInterface* comBus)
{
    uint8_t c = ' ';
    uint8_t payload[COMMANDS::MAX_PACKAGE_LENGTH] = {};
    uint8_t length = 0;
    uint8_t cnt = 0;

    while (1) {
        cnt++;
        if (comBus->hasData()) {
            c = comBus->getChar();

            protocolVersionLastReceivedMessage = static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::UNDEFINED);

            if (c == PROTOCOL::HEADER::SYNC_PATTERN_BYTE_0) {
                length = protocol.searchForMessage((uint8_t*)payload, &protocolVersionLastReceivedMessage);

                if (length > 0) { // found payload
                    if ((protocolVersionLastReceivedMessage == static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::NODE)

                         || protocolVersionLastReceivedMessage
                             == static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::NODE_ENCRYPTED)

                             )
                        && rx_mode_gateway) {
                        sendPayloadToRadioNode(protocol, payload, length);
                    }
                    else {
                        // reset count down to sleep, this may be overwritten by a RaduinoCommandKeepAlive command
                        count_down_ms_to_rfnode_sleep = inactive_time_before_rfnode_sleep_ms;

                        parseCommand(protocol, comBus, payload);
                    }

                    // update entropy pool while we wait for a new command
                    if (lastReceivedCommandWasEncrypted()) {
                        RANDOM::addEntropy(cnt);
                        RANDOM::addEntropyAndMix();
                    }
                }
                c = 0;
            }
        }
        else {
            // idle process
            if (rx_mode_gateway) {
                gatewayIdleLoop(comBus);
            }
            else {
                nodeIdleLoop();
            }
        }
    }
}

bool lastReceivedCommandWasEncrypted()
{
    bool retval = false;
    if ((protocolVersionLastReceivedMessage == static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::GATEWAY_ENCRYPTED))

        || (protocolVersionLastReceivedMessage == static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::NODE_ENCRYPTED))) {
        retval = true;
    }

    return retval;
}

void parseCommand(Protocol& protocol, ComBusInterface* comBus, uint8_t* commandPayload)
{
    commandsParsed++;
    uint8_t responsePayload[COMMANDS::MAX_PAYLOAD_LENGTH] = {};

    responsePayload[0] = static_cast<uint8_t>(COMMANDS::OI::UNDEFINED);

    if ((false == transportEncryptionIsRequired) || lastReceivedCommandWasEncrypted()) {
        commandSwitch(commandPayload, responsePayload, comBus);

        if (responsePayload[0] != static_cast<uint8_t>(COMMANDS::OI::UNDEFINED)) {
            if (false == rx_mode_gateway) {
                _delay_ms(1); // give gateway some time to switch to listening mode
            }

            sendMessage(protocol, comBus, responsePayload);
        }
    }
}

void setKeepAliveInterval(uint8_t interval)
{
    // the value 0 will cause the device to go so sleep immediately
    // a value of 100 will in practize do nothing
    // a value of 200 will double the keep alive time
    count_down_ms_to_rfnode_sleep = interval * 10;
}

void setRequireTransportEncryption(uint8_t isRequired)
{
    if (1 == isRequired) {
        transportEncryptionIsRequired = true;
    }
    else {
        transportEncryptionIsRequired = false;
    }
}

} // namespace
