#pragma once

#include <streamProtocol.hpp>
#include <com_bus.hpp>

class protocol : public StreamProtocol {
public:
    protocol(ComBusInterface* u, CryptoHandlerInterface* c);

    virtual char getChar();
    virtual bool hasData();

private:
    ComBusInterface* m_comBus;
};

