#include <iomanip>
#include <iostream>
#include <protocol.hpp>
#include <string>

LinuxProtocol::LinuxProtocol(const ProtocolCallback& send, const ProtocolCallback& receive, CryptoHandlerInterface* cryptoHandler)
    : m_send(send)
    , m_receive(receive)
      , BufferProtocol(cryptoHandler)
{
}

void LinuxProtocol::send(std::vector<uint8_t> data)
{
    m_send(data); // call m_uart.write(data) via lamba
}

void LinuxProtocol::receive(std::vector<uint8_t> data)
{
    std::vector<uint8_t> packet;
    // called by Uart::execute (includes reading data from serial port)
    for (int i = 0; i < data.size(); i++) {
        m_buffer.push_back(data.at(i));
    }

    if (m_buffer.size() != 0) {
        packet.clear();
        packet = searchForSync(m_buffer);

        if (packet.size() > 0) {
            m_receive(packet);
        }
    }
}

void LinuxProtocol::printDebug(bool d) { m_printDebug = d; }
