#pragma once

#include <bufferProtocol.hpp>
#include <cryptoHandlerInterface.hpp>
#include <epollevent.hpp>
#include <functional>
#include <stdint.h>
#include <vector>

using ProtocolCallback = std::function<void(std::vector<uint8_t>&)>;

class LinuxProtocol : public BufferProtocol {
public:
    LinuxProtocol(const ProtocolCallback& send, const ProtocolCallback& receive, CryptoHandlerInterface* cryptoHandler);

    void receive(std::vector<uint8_t> data);
    void send(std::vector<uint8_t> data);
    void printDebug(bool d);

private:
    std::vector<uint8_t> m_buffer;
    ProtocolCallback m_send;
    ProtocolCallback m_receive;
    bool m_printDebug = false;
};
