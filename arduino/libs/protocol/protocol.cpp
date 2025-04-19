#include <arduinoCryptoHandler.hpp>
#include <protocol.hpp>

Protocol::Protocol(ComBusInterface* comBus, CryptoHandlerInterface* cryptoHandler)
    : StreamProtocol(cryptoHandler)
    , m_comBus(comBus)
{
}

bool Protocol::hasData() { return m_comBus->has_data(); }

char Protocol::getChar() { return m_comBus->getChar(); }

