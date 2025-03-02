#pragma once

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>

#define SPI_DDR DDRB
#define CS PINB1
#define CE PINB2
#define MOSI PINB3
#define MISO PINB4
#define SCK PINB5

void SPI_init();
void SPI_ChipSelectHigh();
void SPI_ChipSelectLow();
void SPI_masterTransmitByte(uint8_t data);
uint8_t SPI_masterReceive();

