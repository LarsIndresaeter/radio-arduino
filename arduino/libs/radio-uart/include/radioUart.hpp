#pragma once
#include <comBusInterface.hpp>
#include <nrf24l01.hpp>
#include <stdint.h>

namespace NRF24L01 {
void rb_put(uint8_t c);
} // namespace

class RadioUart : public ComBusInterface {
public:
    RadioUart();
    virtual void init();
    virtual void putChar(char c);
    virtual void writeBuffer(uint8_t* msg, uint16_t length);
    virtual uint8_t getChar();
    virtual bool hasData();
};

