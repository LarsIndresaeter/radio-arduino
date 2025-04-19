#include <arduinoCryptoHandler.hpp>
#include <protocol.hpp>

Protocol::Protocol(ComBusInterface* u, CryptoHandlerInterface* c)
    : StreamProtocol(c)
    , m_comBus(u)
{
}

bool Protocol::hasData() { return m_comBus->has_data(); }

char Protocol::getChar() { return m_comBus->getChar(); }

