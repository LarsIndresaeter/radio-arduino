#include <arduinoCryptoHandler.hpp>
#include <protocol.hpp>

protocol::protocol(ComBusInterface* u, CryptoHandlerInterface* c)
    : StreamProtocol(c)
    , m_comBus(u)
{
}

bool protocol::hasData() { return m_comBus->has_data(); }

char protocol::getChar() { return m_comBus->getChar(); }

