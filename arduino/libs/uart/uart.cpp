#include <avr/io.h>
#include <stdio.h>
#include <uart.hpp>

#include <avr/interrupt.h>
#include <util/setbaud.h>

/* http://www.cs.mun.ca/~rod/Winter2007/4723/notes/serial/serial.html */

// UBRRn = f_osc / (16*baud) -1
#define UBRR0_1000000 0
#define UBRR0_230400 3
#define UBRR0_57600 16
#define UBRR0_9600 103

uint16_t uart_tx = 0;
uint16_t uart_rx = 0;

namespace UART {
constexpr uint8_t RINGBUFFER_SIZE = 64;
uint8_t ringbuffer[UART::RINGBUFFER_SIZE] = { 0 };
uint8_t rb_length = 0;
uint8_t rb_idx = 0;

void rb_put(uint8_t c)
{
    if (UART::RINGBUFFER_SIZE == UART::rb_length) {
        return;
    }

    UART::ringbuffer[(UART::rb_idx + UART::rb_length++) % UART::RINGBUFFER_SIZE]
        = c;
}

uint8_t rb_get()
{
    uint8_t retval = 0;
    if (0 == UART::rb_length) {
        return retval;
    }

    cli();
    retval = UART::ringbuffer[UART::rb_idx];
    UART::rb_idx = (UART::rb_idx + 1) % UART::RINGBUFFER_SIZE; // advance ptr
    UART::rb_length--;
    sei();

    return retval;
}

} // namespace

Uart::Uart() { init(); }

void Uart::init()
{
    UBRR0H = (uint8_t)(UBRR0_1000000 >> 8); // 57600
    UBRR0L = (uint8_t)(UBRR0_1000000 & 0xff);

    UCSR0A &= ~(_BV(U2X0)); // do not use 2x speed

    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); // 8-bit data
    UCSR0C |= ~(_BV(UMSEL00));          // 0 = asynchronous mode
    // UCSR0C |= _BV(UMSEL00);              // 1 = synchronous mode

    UCSR0B = _BV(RXEN0) | _BV(TXEN0); // Enable RX and TX
    UCSR0B |= _BV(RXCIE0);            // Enable interrupt

    sei();
}

ISR(USART_RX_vect) { UART::rb_put(UDR0); uart_rx++;}

void Uart::putChar(char c)
{
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = c;
    uart_tx++;
}

void Uart::writeBuffer(uint8_t* msg, uint16_t length)
{
    for (uint16_t i = 0; i < length; i++) {
        putChar(msg[i]);
    }
}

uint8_t Uart::getChar()
{
    while (UART::rb_length == 0) {
    } // wait for data to arrive in ring buffer
    return UART::rb_get();
}

bool Uart::has_data() { return UART::rb_length > 0; }
