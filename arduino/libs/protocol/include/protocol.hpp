#pragma once

#include <streamProtocol.hpp>
#include <com_bus.hpp>

class protocol : public StreamProtocol {
public:
    protocol(comBusInterface* u, CryptoHandlerInterface* c);

    virtual char getChar();
    virtual bool hasData();

private:
    comBusInterface* m_comBus;
};

