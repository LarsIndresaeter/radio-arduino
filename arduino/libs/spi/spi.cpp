#include <spi.hpp>

void SPI_init()
{
    // set CS, MOSI and SCK to output
    SPI_DDR |= (1 << CS) | (1 << CE) | (1 << MOSI) | (1 << SCK);

    // enable SPI, set as master, and clock to fosc/16
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
    PORTB |= (1 << CE);

    SPI_ChipSelectHigh();
}

void SPI_ChipSelectHigh() { PORTB |= (1 << CS); }
void SPI_ChipSelectLow() { PORTB &= ~(1 << CS); }

void SPI_masterTransmitByte(uint8_t data)
{
    SPDR = data;

    // Wait for transmission complete
    while (!(SPSR & (1 << SPIF)))
        ;
}

uint8_t SPI_masterReceive()
{
    // transmit dummy byte
    SPDR = 0xFF;

    // Wait for reception complete
    while (!(SPSR & (1 << SPIF)))
        ;

    // return Data Register
    return SPDR;
}
