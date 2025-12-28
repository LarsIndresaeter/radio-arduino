#include <radioUart.hpp>
#include <util/delay.h>

namespace RINGBUFFER {
constexpr uint8_t RINGBUFFER_SIZE = 3*NRF24L01_PACKET_SIZE;
uint8_t ringbuffer[RINGBUFFER::RINGBUFFER_SIZE] = { 0 };
uint8_t rb_length = 0;
uint8_t rb_idx = 0;

void rb_put(uint8_t c)
{
    if (RINGBUFFER::RINGBUFFER_SIZE == RINGBUFFER::rb_length) {
        return;
    }

    RINGBUFFER::ringbuffer[(RINGBUFFER::rb_idx + RINGBUFFER::rb_length++) % RINGBUFFER::RINGBUFFER_SIZE] = c;
}

uint8_t rb_get()
{
    uint8_t retval = 0;
    if (0 == RINGBUFFER::rb_length) {
        return retval;
    }

    retval = RINGBUFFER::ringbuffer[RINGBUFFER::rb_idx];
    RINGBUFFER::rb_idx = (RINGBUFFER::rb_idx + 1) % RINGBUFFER::RINGBUFFER_SIZE; // advance ptr
    RINGBUFFER::rb_length--;

    return retval;
}

} // namespace

RadioUart::RadioUart() {}

void RadioUart::init() {}

void RadioUart::putChar(char c) { RINGBUFFER::rb_put(c); }

void RadioUart::writeBuffer(uint8_t* msg, uint16_t length) { NRF24L01_tx(&msg[0], length); }

uint8_t RadioUart::getChar()
{
    hasData(); // force read if ring buffer is empty

    return RINGBUFFER::rb_get();
}

bool RadioUart::hasData()
{
    uint8_t rx_buf[NRF24L01_PACKET_SIZE] = { 0 };

    if (RINGBUFFER::rb_length == 0) {
        NRF24L01_write_register(NRF24L01_REGISTER_STATUS, 0x70); // clear RX_DR, TX_DS and MAX_TR

        uint8_t status = NRF24L01_read_register(NRF24L01_REGISTER_STATUS);

        if ((status & 0x0E) != 0x0E) { // rx_fifo not empty
            uint8_t read_length = NRF24L01_rx(&rx_buf[0]);

            for (uint8_t i = 0; i < read_length; i++) {
                putChar(rx_buf[i]);
            }
            NRF24L01_write_register(NRF24L01_REGISTER_STATUS,
                                    0x70); // clear RX_DR, TX_DS and MAX_TR
        }
    }

    return RINGBUFFER::rb_length > 0;
}
