#include <nrf24l01.hpp>
#include <spi.hpp>
#include <util/delay.h>

uint16_t rf_tx = 0;
uint16_t rf_rx = 0;

uint8_t rx_tx_addr[5] = { 0xF0, 0xF0, 0xF0, 0xF0, node_address };
uint8_t rf_channel = 121;

uint8_t node_address = 0;

// do not expose these functions
void NRF24L01_wait_for_tx_complete();

#ifdef USE_NRF24L01_INTTERRUPT
ISR(PCINT0_vect)
{
    uint8_t rx_buf[NRF24L01_PACKET_SIZE] = { 0 };

    NRF24L01_write_register(
        NRF24L01_REGISTER_STATUS, 0x70); // clear RX_DR, TX_DS and MAX_TR

    uint8_t status = NRF24L01_read_register(NRF24L01_REGISTER_STATUS);

    if ((status & 0x0E) != 0x0E) { // rx_fifo not empty
        uint8_t read_length
            = NRF24L01_rx(&rx_buf[0]);

        for (uint8_t i = 0; i < read_length; i++) {
            NRF24L01::rb_put(rx_buf[i]);
        }
        NRF24L01_write_register(
            NRF24L01_REGISTER_STATUS,
            0x70); // clear RX_DR, TX_DS and MAX_TR
    }
}
#endif

void NRF24L01_write_register(uint8_t reg, uint8_t value)
{
    SPI_ChipSelectLow();

    SPI_masterTransmitByte(NRF24L01_W_REGISTER | reg);
    SPI_masterTransmitByte(value);

    SPI_ChipSelectHigh();
}

void NRF24L01_write_register(uint8_t reg, uint8_t* arr, uint8_t length)
{
    SPI_ChipSelectLow();

    SPI_masterTransmitByte(NRF24L01_W_REGISTER | reg);

    for (uint8_t i = 0; i < length; i++) {
        SPI_masterTransmitByte(arr[i]);
    }

    SPI_ChipSelectHigh();
}

void NRF24L01_flush_tx()
{
    SPI_ChipSelectLow();

    SPI_masterTransmitByte(NRF24L01_FLUSH_TX);

    SPI_ChipSelectHigh();
}

void NRF24L01_flush_rx()
{
    SPI_ChipSelectLow();

    SPI_masterTransmitByte(NRF24L01_FLUSH_RX);

    SPI_ChipSelectHigh();
}

uint8_t NRF24L01_read_register(uint8_t reg)
{
    SPI_ChipSelectLow();

    SPI_masterTransmitByte(reg);
    return SPI_masterReceive();

    SPI_ChipSelectHigh();
}

void NRF24L01_power_down()
{
    PORTB &= ~(1 << CE); // disable nrf24l01
}

void NRF24L01_power_up()
{
    PORTB |= 1 << CE; // enable nrf24l01
}

void NRF24L01_set_rx_as_master(bool master)
{
    uint8_t reg = 0x32; // mask interrupts, EN_CRC=1, PWR_UP=1

    if (master) {
        reg &= 0xfe; // PRIM_RX=0
    }
    else {
        reg |= 0x01; // PRIM_RX=1
    }

    // NRF24L01_write_register(
    // NRF24L01_REGISTER_STATUS, 0x70); // clear RX_DR, TX_DS and MAX_TR
        
    NRF24L01_write_register(NRF24L01_REGISTER_CONFIG, reg);
    NRF24L01_flush_rx();
    NRF24L01_flush_tx();
}

void NRF24L01_init(uint8_t* rx_addr, uint8_t* tx_addr, uint8_t ch, bool master)
{
    SPI_init();

    NRF24L01_write_register(NRF24L01_REGISTER_CONFIG,
                            0x00); // power down

    NRF24L01_flush_tx();

    NRF24L01_write_register(
        NRF24L01_REGISTER_EN_AA,
        0x01); // Disable auto acknowledgement on data pipe 0
    NRF24L01_write_register(
        NRF24L01_REGISTER_SETUP_RETR,
        0xFF); // 750 uS retransmit delay, 3 retransmit count
    NRF24L01_write_register(
        NRF24L01_REGISTER_EN_RXADDR, 0x03); // enable RX on pipe 0
    NRF24L01_write_register(
        NRF24L01_REGISTER_DYNPD,
        0x03); // enable dynamic payload length on pipe 0
    NRF24L01_write_register(NRF24L01_REGISTER_RF_CHANNEL, ch);
    NRF24L01_write_register(
        NRF24L01_REGISTER_RF_SETUP,
        0x06); // 1MBPS, 0dBm (max power)
    NRF24L01_write_register(
        NRF24L01_REGISTER_STATUS, 0x70); // clear RX_DR, TX_DS and MAX_TR
    NRF24L01_write_register(
        NRF24L01_REGISTER_FEATURE,
        0x07); // enable dynamic payload length and payload with ACK

    NRF24L01_write_register(
        NRF24L01_REGISTER_RX_ADDR_P0, &rx_addr[0], NRF24L01_ADDR_SIZE);

    NRF24L01_write_register(
        NRF24L01_REGISTER_TX_ADDR, &tx_addr[0], NRF24L01_ADDR_SIZE);

    NRF24L01_set_rx_as_master(master);

#ifdef USE_NRF24L01_INTTERRUPT
    PCICR |= _BV(PCIE0);
    PCMSK0 |= _BV(PCINT0);
#endif
}

uint8_t NRF24L01_read_rx_payload(uint8_t* arr)
{
    uint8_t length = 0;

    // read FIFO_STATUS
    SPI_ChipSelectLow();

    SPI_masterTransmitByte(NRF24L01_REGISTER_FIFO_STATUS);
    uint8_t fifo_status = SPI_masterReceive();

    SPI_ChipSelectHigh();

    // check if there is data in the RX_FIFO
    if (0 == (fifo_status & 0x01)) {
        SPI_ChipSelectLow();

        SPI_masterTransmitByte(NRF24L01_R_RX_PL_WID);
        length = SPI_masterReceive(); // read length of RX_FIFO

        SPI_ChipSelectHigh();
    }

    if (length > 0) {
        // read RX_FIFO
        SPI_ChipSelectLow();

        SPI_masterTransmitByte(NRF24L01_R_RX_PAYLOAD);

        for (uint8_t i = 0; i < length; i++) {
            arr[i] = SPI_masterReceive();
        }

        SPI_ChipSelectHigh();
    }

    rf_rx += length;

    return length;
}

void NRF24L01_write_tx_payload(uint8_t* arr, uint8_t length)
{
    rf_tx += length;

    if (length > NRF24L01_PACKET_SIZE) {
        length = NRF24L01_PACKET_SIZE;
    }

    SPI_ChipSelectLow();

    SPI_masterTransmitByte(NRF24L01_W_TX_PAYLOAD);

    for (uint8_t i = 0; i < length; i++) {
        SPI_masterTransmitByte(arr[i]);
    }

    SPI_ChipSelectHigh();
}

void NRF24L01_write_ack_payload(uint8_t* arr, uint8_t length)
{
    if (length > NRF24L01_PACKET_SIZE) {
        length = NRF24L01_PACKET_SIZE;
    }

    SPI_ChipSelectLow();

    SPI_masterTransmitByte(NRF24L01_W_ACK_PAYLOAD_P0);

    for (uint8_t i = 0; i < length; i++) {
        SPI_masterTransmitByte(arr[i]);
    }

    SPI_ChipSelectHigh();
}

void NRF24L01_tx(uint8_t* tx_buffer, uint8_t length)
{
    rf_tx += length;

    NRF24L01_write_register(
        NRF24L01_REGISTER_STATUS, 0x70); // clear RX_DR, TX_DS and MAX_TR

    NRF24L01_set_rx_as_master(true);

    uint8_t idx = 0;
    while (length > idx) {
        if ((length - idx) > 32) {
            NRF24L01_write_tx_payload(&tx_buffer[idx], 32);
            idx += 32;
        }
        else {
            NRF24L01_write_tx_payload(&tx_buffer[idx], (length - idx));
            idx = length;
        }
    }

    NRF24L01_wait_for_tx_complete();

    // seher
    NRF24L01_write_register(
        NRF24L01_REGISTER_STATUS, 0x70); // clear RX_DR, TX_DS and MAX_TR

    NRF24L01_set_rx_as_master(false);
}

void NRF24L01_wait_for_tx_complete()
{
    SPI_ChipSelectLow();

    for (int i = 0; i < 100; i++) {
        SPI_masterTransmitByte(NRF24L01_REGISTER_STATUS);
        uint8_t fifo_status = SPI_masterReceive();
        if (fifo_status & 0x20) {
            // TX_DS flag is set when AUTO_ACK is activated and ACK received
            break;
        }
    }

    SPI_ChipSelectHigh();
}

uint8_t NRF24L01_rx(uint8_t* rx_buffer)
{
    uint8_t response_length = 0;
    uint8_t segment_length = 0;

#ifndef USE_NRF24L01_INTTERRUPT
    for (uint16_t i = 0; i < 3; i++) {
        segment_length = NRF24L01_read_rx_payload(&rx_buffer[response_length]);
        response_length += segment_length;

        if (response_length > 4) {
            uint8_t msg_length = rx_buffer[3];
            if ((response_length >= (8 + msg_length)) && rx_buffer[0] == 0xFE) {
                i = 10000;
                break;
            }
        }
        //_delay_ms(10);
    }
#endif

    return response_length;
}

