#include "fakeStreamProtocol.hpp"

FakeStreamProtocol::FakeStreamProtocol()
    : StreamProtocol(&m_cryptoHandler) {};

void FakeStreamProtocol::appendData(std::vector<uint8_t> data)
{
    for (int i = 0; i < data.size(); i++) {
        m_data.push_back(data.at(i));
    }
}

char FakeStreamProtocol::getChar()
{
    uint8_t c = m_data.at(0);
    m_data.erase(m_data.begin());
    // std::cout << "DEBUG: *** getChar() ******" << std::endl;
    return c;
}

bool FakeStreamProtocol::hasData() { return (m_data.size() > 0); }

