#pragma once
#include <com_bus.hpp>
#include <stdint.h>
#include <stdio.h>

/* http://www.ermicro.com/blog/?p=325 */

class uart : public comBusInterface{
public:
    uart();
    virtual void init();
    virtual void putChar(char c);
    virtual void writeBuffer(uint8_t* msg, uint16_t length);
    virtual uint8_t getChar();
    virtual bool has_data();
};

