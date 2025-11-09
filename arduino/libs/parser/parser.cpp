#include <parser.hpp>

extern bool rx_mode_gateway;

namespace PARSER {
uint16_t commandsParsed = 0;

bool transportEncryptionIsRequired = true;

uint32_t keep_alive_interval_ms = 100; // time in idle loop before entering sleep
uint32_t idle_loop_cnt_ms = 0;

uint8_t protocolVersionLastReceivedMessage = static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::UNDEFINED);

uint16_t getCommandsParsedCounter() { return commandsParsed; }

void sendMessage(Protocol protocol, ComBusInterface* comBus, uint8_t* payload)
{
    uint8_t packet[COMMANDS::MAX_PACKAGE_LENGTH];
    uint8_t length = payload[1] + 2;

    if (protocolVersionLastReceivedMessage == static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::GATEWAY_ENCRYPTED)
        || ((rx_mode_gateway == false)
            && protocolVersionLastReceivedMessage
                == static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::NODE_ENCRYPTED))

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
            && protocolVersionLastReceivedMessage
                == static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::NODE))) {
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
    uint8_t packet[COMMANDS::MAX_PACKAGE_LENGTH];
    uint8_t data_size = 0;

    if (protocolVersionLastReceivedMessage == static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::NODE)
        && rx_mode_gateway) {
        protocol.createPacket(length, payload, &packet[0], protocolVersionLastReceivedMessage);

        data_size = PROTOCOL::HEADER::LENGTH + PROTOCOL::CHECKSUM::LENGTH + length;

        // send command to rx_node and wait for response
        NRF24L01_tx(&packet[0], data_size);
    }
    else if (
        protocolVersionLastReceivedMessage
            == static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::NODE_ENCRYPTED)
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
    idle_loop_cnt_ms++;
    if (idle_loop_cnt_ms > keep_alive_interval_ms) {
        // node has been idle for too long so go to sleep and wait for wakeup command
        SLEEP::rfNodeSleepAndPollForWakeup();
        idle_loop_cnt_ms = 0;
    }
}

void gatewayIdleLoop(ComBusInterface* comBus)
{
    // rx gateway reads response in idle loop
    uint8_t ack_packet[32];
    uint8_t response_length = NRF24L01_rx(&ack_packet[0]);

    uint8_t is_wakeup_ack = RADIOLINK::isDiscoverPackage(response_length, &ack_packet[0]);

    if (is_wakeup_ack == 0) {
        comBus->writeBuffer(&ack_packet[0], response_length);
    }
}

void parseInput(Protocol protocol, ComBusInterface* comBus)
{
    uint8_t c = ' ';
    uint8_t payload[COMMANDS::MAX_PAYLOAD_LENGTH] = {};
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
                    RANDOM::addEntropy(cnt);

                    if ((protocolVersionLastReceivedMessage
                             == static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::NODE)

                         || protocolVersionLastReceivedMessage
                             == static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::NODE_ENCRYPTED)

                             )
                        && rx_mode_gateway) {
                        sendPayloadToRadioNode(protocol, payload, length);
                    }
                    else {
                        parseCommand(protocol, comBus, payload);

                        if (false == rx_mode_gateway) {
                            idle_loop_cnt_ms = 0;
                        }
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
    if ((protocolVersionLastReceivedMessage
         == static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::GATEWAY_ENCRYPTED))

        || (protocolVersionLastReceivedMessage
            == static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::NODE_ENCRYPTED))) {
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
    if (false == rx_mode_gateway) {
        keep_alive_interval_ms = 100 + interval * 100;

        // TODO: this is probably not what you want
        if (0 == interval) {
            // if keep alive interval is set to minimum the go to sleep immediately
            idle_loop_cnt_ms = keep_alive_interval_ms;
        }
    }
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
