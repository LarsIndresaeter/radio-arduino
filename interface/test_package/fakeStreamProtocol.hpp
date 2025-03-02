#pragma once

#include <streamProtocol.hpp>
#include <vector>
#include "fakeCryptoHandler.hpp"

class FakeStreamProtocol : public StreamProtocol {
public:
    FakeStreamProtocol();
    void appendData(std::vector<uint8_t> data);
    virtual char getChar();
    virtual bool hasData();

private:
    FakeCryptoHandler m_cryptoHandler;
    std::vector<uint8_t> m_data;
};

