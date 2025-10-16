#pragma once

#include <streamProtocol.hpp>
#include <comBusInterface.hpp>

class Protocol : public StreamProtocol {
public:
    Protocol(ComBusInterface* comBus, CryptoHandlerInterface* cryptoHandler);

    virtual char getChar();
    virtual bool hasData();

private:
    ComBusInterface* m_comBus;
};

