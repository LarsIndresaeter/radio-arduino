#pragma once

#include "fakeBufferProtocol.hpp"
#include "fakeCryptoHandler.hpp"
#include <bufferProtocol.hpp>

class FakeBufferProtocol : public BufferProtocol {
public:
    FakeBufferProtocol();

private:
    FakeCryptoHandler m_cryptoHandler;
};
