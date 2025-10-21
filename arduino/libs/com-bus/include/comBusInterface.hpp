#pragma once
#include <stdint.h>

class ComBusInterface {
public:
    virtual void init() = 0;
    virtual void putChar(char c) = 0;
    virtual void writeBuffer(uint8_t* msg, uint16_t length) = 0;
    virtual uint8_t getChar() = 0;
    virtual bool hasData() = 0;
};
