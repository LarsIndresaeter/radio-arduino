#pragma once

#include <stdint.h>

#define NRF24L01_REGISTER_CONFIG 0x00
#define NRF24L01_REGISTER_EN_AA 0x01
#define NRF24L01_REGISTER_EN_RXADDR 0x02
#define NRF24L01_REGISTER_SETUP_AW 0x03
#define NRF24L01_REGISTER_SETUP_RETR 0x04
#define NRF24L01_REGISTER_RF_CHANNEL 0x05
#define NRF24L01_REGISTER_RF_SETUP 0x06
#define NRF24L01_REGISTER_STATUS 0x07
#define NRF24L01_REGISTER_OBSERVE_TX 0x08
#define NRF24L01_REGISTER_RX_ADDR_P0 0x0A
#define NRF24L01_REGISTER_RX_ADDR_P1 0x0B
#define NRF24L01_REGISTER_TX_ADDR 0x10
#define NRF24L01_REGISTER_RX_PW_P0 0x11
#define NRF24L01_REGISTER_RX_PW_P1 0x12
#define NRF24L01_REGISTER_FIFO_STATUS 0x17
#define NRF24L01_REGISTER_DYNPD 0x1C
#define NRF24L01_REGISTER_FEATURE 0x1D

#define NRF24L01_W_REGISTER 0b00100000
#define NRF24L01_R_RX_PAYLOAD 0b01100001
#define NRF24L01_W_TX_PAYLOAD 0b10100000
#define NRF24L01_W_TX_PAYLOAD_NO_ACK 0b10110000
#define NRF24L01_FLUSH_TX 0b11100001
#define NRF24L01_FLUSH_RX 0b11100010
#define NRF24L01_R_RX_PL_WID 0b01100000
#define NRF24L01_W_ACK_PAYLOAD_P0 0b10101000
#define NRF24L01_W_ACK_PAYLOAD_P1 0b10101001

#define NRF24L01_PACKET_SIZE 32
#define NRF24L01_ADDR_SIZE 5

void NRF24L01_power_down();
void NRF24L01_power_up();
void NRF24L01_flush_tx();
void NRF24L01_flush_rx();
void NRF24L01_set_rx_as_master(bool master);
uint8_t NRF24L01_read_rx_payload(uint8_t* arr);
void NRF24L01_write_tx_payload(uint8_t* arr, uint8_t length);
void NRF24L01_write_ack_payload(uint8_t* arr, uint8_t length);
void NRF24L01_init(uint8_t* rx_addr, uint8_t* tx_addr, bool master);
void NRF24L01_write_register(uint8_t reg, uint8_t value);
void NRF24L01_write_register(uint8_t reg, uint8_t* arr, uint8_t length);
uint8_t NRF24L01_read_register(uint8_t reg);
void NRF24L01_tx(uint8_t* tx_buffer, uint8_t length);
uint8_t NRF24L01_rx(uint8_t* rx_buffer);

void NRF24L01_set_rf_channel(uint8_t ch);
