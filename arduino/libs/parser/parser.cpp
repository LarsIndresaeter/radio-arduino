#include <parser.hpp>

Random random;

#ifdef REPLACE_UART_WITH_RADIO_COMMUNICATION_AKA_RX_NODE
bool rx_mode_gateway = false;
#else
bool rx_mode_gateway = true;
#endif

uint16_t commandsParsed = 0;

uint32_t keep_alive_interval_ms = 100; // time in idle loop before entering sleep
uint32_t idle_loop_cnt_ms = 0;

uint8_t protocolVersionLastReceivedMessage
    = static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::UNDEFINED);

void sendMessage(Protocol protocol, ComBusInterface* comBus, uint8_t* payload)
{
    uint8_t packet[COMMANDS::MAX_PACKAGE_LENGTH];
    uint8_t length = payload[1] + 2;

    if (protocolVersionLastReceivedMessage
            == static_cast<uint8_t>(
                PROTOCOL::HEADER::VERSION::ENCRYPTED_BINARY_AND_TEXT)
        || ((rx_mode_gateway == false)
            && protocolVersionLastReceivedMessage
                == static_cast<uint8_t>(
                    PROTOCOL::HEADER::VERSION::RADIO_ENCRYPTED_BINARY_AND_TEXT))

    ) {
        protocol.createEncryptedPacket(
            length, payload, &packet[0], protocolVersionLastReceivedMessage);

        if (false == rx_mode_gateway) {
            NRF24L01_tx(
                &packet[0],
                PROTOCOL::HEADER::LENGTH + PROTOCOL::CHECKSUM::LENGTH + length
                    + PROTOCOL::ENCRYPTED::CRYPTO_OVERHEAD);
        }
        else {
            comBus->writeBuffer(
                &packet[0],
                PROTOCOL::HEADER::LENGTH + PROTOCOL::CHECKSUM::LENGTH + length
                    + PROTOCOL::ENCRYPTED::CRYPTO_OVERHEAD);
        }
    }
    else if (
        protocolVersionLastReceivedMessage
            == static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::BINARY_AND_TEXT)
        || ((rx_mode_gateway == false)
            && protocolVersionLastReceivedMessage
                == static_cast<uint8_t>(
                    PROTOCOL::HEADER::VERSION::RADIO_BINARY_AND_TEXT))) {
        protocol.createPacket(
            length, payload, &packet[0], protocolVersionLastReceivedMessage);

        if (false == rx_mode_gateway) {
            NRF24L01_tx(
                &packet[0],
                PROTOCOL::HEADER::LENGTH + PROTOCOL::CHECKSUM::LENGTH + length);
        }
        else {
            comBus->writeBuffer(
                &packet[0],
                PROTOCOL::HEADER::LENGTH + PROTOCOL::CHECKSUM::LENGTH + length);
        }
    }
}

void sendPayloadToRadioNode(Protocol protocol, uint8_t *payload, uint8_t length)
{
    uint8_t packet[COMMANDS::MAX_PACKAGE_LENGTH];
    uint8_t data_size = 0;

    if (protocolVersionLastReceivedMessage
            == static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::
                    RADIO_BINARY_AND_TEXT)
        && rx_mode_gateway) {

        protocol.createPacket(
            length,
            payload,
            &packet[0],
            protocolVersionLastReceivedMessage);

        data_size = PROTOCOL::HEADER::LENGTH
            + PROTOCOL::CHECKSUM::LENGTH + length;

        // send command to rx_node and wait for response
        NRF24L01_tx(&packet[0], data_size);
    }
    else if (
        protocolVersionLastReceivedMessage
            == static_cast<uint8_t>(
                PROTOCOL::HEADER::VERSION::
                    RADIO_ENCRYPTED_BINARY_AND_TEXT)
        && rx_mode_gateway) {
        protocol.createEncryptedPacket(
            length,
            payload,
            &packet[0],
            protocolVersionLastReceivedMessage);

        data_size = PROTOCOL::HEADER::LENGTH
            + PROTOCOL::CHECKSUM::LENGTH + length
            + PROTOCOL::ENCRYPTED::CRYPTO_OVERHEAD;

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
        rxNodeSleepAndPollForWakeup();
        idle_loop_cnt_ms = 0;
    }
}

void gatewayIdleLoop(ComBusInterface* comBus)
{
    // rx gateway reads response in idle loop
    uint8_t ack_packet[32];
    uint8_t response_length
        = NRF24L01_rx(&ack_packet[0]);

    // ignore messages from rx node if it is a wakeup ack packet
    uint8_t is_wakeup_ack = 0;
    if (response_length == 32) {
        is_wakeup_ack = 1;
        for (uint8_t j = 0; j < 31; j++) {
            if (ack_packet[j] != rf_link_discover_package[j]) {
                is_wakeup_ack = 0;
            }
        }
    }

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
        if (comBus->has_data()) {
            c = comBus->getChar();

            protocolVersionLastReceivedMessage
                = static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::UNDEFINED);

            if (c == PROTOCOL::HEADER::SYNC_PATTERN_BYTE_0) {
                length = protocol.searchForMessage(
                    (uint8_t*)payload, &protocolVersionLastReceivedMessage);

                if (length > 0) { // found payload
                    random.addEntropy(cnt);

                    if ((protocolVersionLastReceivedMessage
                                == static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::
                                        RADIO_BINARY_AND_TEXT)

                            || protocolVersionLastReceivedMessage
                                == static_cast<uint8_t>(
                                    PROTOCOL::HEADER::VERSION::
                                        RADIO_ENCRYPTED_BINARY_AND_TEXT)

                                )
                        && rx_mode_gateway) {

                        sendPayloadToRadioNode(protocol, payload, length);
                    }
                    else {
                        parseCommand(protocol, comBus, payload);

                        if (false == rx_mode_gateway)
                        {
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

void parseCommand(
    Protocol& protocol, ComBusInterface* comBus, uint8_t* commandPayload)
{
    commandsParsed++;
    uint8_t responsePayload[COMMANDS::MAX_PAYLOAD_LENGTH] = {};

    responsePayload[0] = static_cast<uint8_t>(COMMANDS::OI::UNDEFINED);

    commandSwitch(commandPayload, responsePayload, comBus);

    if (responsePayload[0] != static_cast<uint8_t>(COMMANDS::OI::UNDEFINED)) {
        if (false == rx_mode_gateway) {
            _delay_ms(1); // give gateway some time to switch to listening mode
        }

        sendMessage(protocol, comBus, responsePayload);
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

uint8_t wakeupCommand(uint8_t checkAttentionFlag)
{
    uint8_t read_discover_package[32] = { 0 };
    uint8_t attention_flag = 0;

    if (rx_mode_gateway) {
        for (uint16_t i = 0; i < 1000; i++) {
            uint8_t length = NRF24L01_read_rx_payload(&read_discover_package[0]);

            if (length == 32) {
                attention_flag = read_discover_package[31];

                if ((0 != checkAttentionFlag) && (0 == read_discover_package[31])) {
                    // received discover package but about wakeup since data available flag was not set
                    break;
                }
                else {
                    NRF24L01_tx(&rf_link_wakeup_command[0], 32);

                    for (uint8_t j = 0; j < 31; j++) {
                        if (read_discover_package[j] != rf_link_discover_package[j]) {
                            // set status
                        }
                        i = 10000;
                        break;
                    }
                }
            }

            _delay_ms(10);
        }
        _delay_ms(10); // give rf node some time to be ready for new commands
    }

return attention_flag;
}
