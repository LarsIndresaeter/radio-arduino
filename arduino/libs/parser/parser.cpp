#include <parser.hpp>

extern uint8_t attention_flag;

extern uint8_t node_address;

Random random;

#ifdef REPLACE_UART_WITH_RADIO_COMMUNICATION_AKA_RX_NODE
bool rx_mode_gateway = false;
#else
bool rx_mode_gateway = true;
#endif

uint8_t rf_link_wakeup_command[32] 
        = {'w', 'a', 'k', 'e', 'u', 'p', ' ', 0x55, 0x55, 0x55, 0x55, 
            0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
            0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, node_address};

extern uint8_t rf_link_discover_package[32];
        //= { 'd', 'i', 's', 'c', 'o', 'v', 'e', 'r', ' ', 0xaa, 0xaa,
            //0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
            //0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, node_address, 1 };


uint8_t protocolVersionLastReceivedMessage
    = static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::UNDEFINED);


void rxNodeSleepAndPollForWakeup()
{
    // periodically poll rx gateway for wakeup command

    uint8_t read_wakeup_command[32] = { 0 };

    uint8_t wakeup_received = 0;

    while (wakeup_received == 0) {
        powerDownRadioAndSleep(5000);

        // send command to rf gateway
        rf_link_discover_package[31] = attention_flag;
        NRF24L01_tx(&rf_link_discover_package[0], 32);

        _delay_ms(10);

        // poll gateway for wakeup command in ack packet
        uint8_t length = NRF24L01_read_rx_payload(&read_wakeup_command[0]);

        if (length == 32) {
            wakeup_received = 1;
            for (uint8_t i = 0; i < 32; i++) {
                if (read_wakeup_command[i] != rf_link_wakeup_command[i]) {
                    wakeup_received = 0;
                }
            }
        }
    }
}

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

#ifdef REPLACE_UART_WITH_RADIO_COMMUNICATION_AKA_RX_NODE
        NRF24L01_tx(
            &packet[0],
            PROTOCOL::HEADER::LENGTH + PROTOCOL::CHECKSUM::LENGTH + length
                + PROTOCOL::ENCRYPTED::CRYPTO_OVERHEAD);
#else
        comBus->writeBuffer(
            &packet[0],
            PROTOCOL::HEADER::LENGTH + PROTOCOL::CHECKSUM::LENGTH + length
                + PROTOCOL::ENCRYPTED::CRYPTO_OVERHEAD);
#endif
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

#ifdef REPLACE_UART_WITH_RADIO_COMMUNICATION_AKA_RX_NODE
        NRF24L01_tx(
            &packet[0],
            PROTOCOL::HEADER::LENGTH + PROTOCOL::CHECKSUM::LENGTH + length);
#else
        comBus->writeBuffer(
            &packet[0],
            PROTOCOL::HEADER::LENGTH + PROTOCOL::CHECKSUM::LENGTH + length);
#endif
    }
}

void parseInput(Protocol protocol, ComBusInterface* comBus)
{
    uint8_t c = ' ';
    uint8_t payload[COMMANDS::MAX_PAYLOAD_LENGTH] = {};
    uint8_t packet[COMMANDS::MAX_PACKAGE_LENGTH];
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

                    if (protocolVersionLastReceivedMessage
                            == static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::
                                                        RADIO_BINARY_AND_TEXT)
                        && rx_mode_gateway) {
                        uint8_t data_size = 0;
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
                        uint8_t data_size = 0;
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
                    else {
                        parseCommand(protocol, comBus, payload);
#ifdef REPLACE_UART_WITH_RADIO_COMMUNICATION_AKA_RX_NODE
                        idle_loop_cnt_ms = 0;
#endif
                    }
                }
                c = 0;
            }
        }
        else {
            // idle process
#ifdef USE_NRF24L01_INTTERRUPT
            RadioUart uartRadio;
            if (uartRadio.has_data()) {
                comBus->putChar(uartRadio.getChar());
            }

#ifdef REPLACE_UART_WITH_RADIO_COMMUNICATION_AKA_RX_NODE
            set_sleep_mode(SLEEP_MODE_PWR_DOWN);
            sleep_enable();
            sleep_bod_disable();
            sei();
            sleep_cpu();
            sleep_disable();
            cli();
#endif
#endif

            
#ifdef REPLACE_UART_WITH_RADIO_COMMUNICATION_AKA_RX_NODE
            _delay_ms(1);
            idle_loop_cnt_ms++;
            if (idle_loop_cnt_ms > keep_alive_interval_ms) {
                // node has been idle for too long so go to sleep and wait for wakeup command
                rxNodeSleepAndPollForWakeup();
                idle_loop_cnt_ms = 0;
            }
#endif

#ifndef REPLACE_UART_WITH_RADIO_COMMUNICATION_AKA_RX_NODE
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

            if(is_wakeup_ack == 0)
            {
                comBus->writeBuffer(&ack_packet[0], response_length);
            }
#endif
        }
    }
}

