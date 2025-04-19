#pragma once

#include <streamProtocol.hpp>
#include <com_bus.hpp>

class Protocol : public StreamProtocol {
public:
    Protocol(ComBusInterface* u, CryptoHandlerInterface* c);

    virtual char getChar();
    virtual bool hasData();

private:
    ComBusInterface* m_comBus;
};

