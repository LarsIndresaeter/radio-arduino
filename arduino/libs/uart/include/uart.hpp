#pragma once
#include <comBusInterface.hpp>
#include <stdint.h>
#include <stdio.h>

/* http://www.ermicro.com/blog/?p=325 */

namespace UART {
uint32_t getUartTxBytes();
uint32_t getUartRxBytes();
}

class Uart : public ComBusInterface {
public:
    Uart();
    virtual void init();
    virtual void putChar(char c);
    virtual void writeBuffer(uint8_t* msg, uint16_t length);
    virtual uint8_t getChar();
    virtual bool hasData();
};
