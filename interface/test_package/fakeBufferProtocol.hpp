#pragma once

#include <bufferProtocol.hpp>
#include "fakeBufferProtocol.hpp"
#include "fakeCryptoHandler.hpp"

class FakeBufferProtocol : public BufferProtocol {
public:
    FakeBufferProtocol();
private:
    FakeCryptoHandler m_cryptoHandler;
};

