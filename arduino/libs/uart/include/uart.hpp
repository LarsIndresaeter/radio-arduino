#pragma once
#include <com_bus.hpp>
#include <stdint.h>
#include <stdio.h>

/* http://www.ermicro.com/blog/?p=325 */

extern uint32_t uart_tx;
extern uint32_t uart_rx;

class Uart : public ComBusInterface{
public:
    Uart();
    virtual void init();
    virtual void putChar(char c);
    virtual void writeBuffer(uint8_t* msg, uint16_t length);
    virtual uint8_t getChar();
    virtual bool has_data();
};

