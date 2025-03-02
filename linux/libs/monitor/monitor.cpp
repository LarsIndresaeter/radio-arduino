#include <chrono>
#include <eventprocess.hpp>
#include <iomanip>
#include <iostream>
#include <monitor.hpp>
#include <thread>

using namespace std::chrono_literals;

monitor::monitor(Uart& uart, EventProcess& ep, CryptoHandlerInterface* cryptoHandler)
    : m_uart(uart)
    , m_ep(ep)
    , m_parser(
          [this](const std::vector<uint8_t>& data) { m_bytesSent += data.size(); m_uart.write(data); },
          [this](const std::vector<uint8_t>& data) { reply(data); }, cryptoHandler)
{
    m_uart.setReadHandler([&](auto data) { m_bytesReceived += data.size(); m_parser.receive(data); });
    m_ep.addEvent(&m_uart, EPOLLIN);
    m_validResponseCounter = 0;
    m_inValidResponseCounter = 0;
    m_commandsSentCounter = 0;
    m_bytesSent = 0;
    m_bytesReceived = 0;
}

void monitor::setPrintResponseTime(bool value) { m_printResponseTime = value; }

void monitor::setTransportEncryption(bool value)
{
    m_transportEncryption = value;
};

void monitor::setResponseCallback(uint8_t cmd, ProtocolCallback cb)
{
    m_responseCallback.emplace(
        cmd, cb); // set response callback for message with this command id
}

void monitor::sendRequest(COMMANDS::OI cmd, std::vector<uint8_t> data, uint8_t protocol_version)
{
    std::unique_lock<std::mutex> lock(m_mutex);

    std::vector<uint8_t> byteString;
    byteString.push_back(static_cast<uint8_t>(cmd));
    byteString.insert(byteString.end(), data.begin(), data.end());

    m_parser.send(m_parser.createBinaryCommand(byteString, protocol_version));
    m_commandsSentCounter++;
}

const void monitor::reply(std::vector<uint8_t> data)
{
    if(data.size() < 5)
    {
        return; // we want to read command id from byte 4
    }

    std::unique_lock<std::mutex> lock(m_mutex);

    uint8_t cmd = data.at(4);

    std::map<uint8_t, ProtocolCallback>::iterator it;
    it = m_responseCallback.find(cmd); // find callback for this message
    if (it != m_responseCallback.end()) {
        it->second(data); // calls UartCommandBase::setResponse
        m_responseCallback.erase(it);
        m_validResponseCounter++;
    }

    m_cv.notify_one(); // notify get<> of received data
}

void monitor::get(std::vector<uint8_t> command, uint8_t protocol_version)
{
    // used by get<> to send binary data
    COMMANDS::OI cmd = static_cast<COMMANDS::OI>(command.at(0));

    // pop command byte
    command.erase(command.begin());

    sendRequest(cmd, command, protocol_version);
}

void monitor::printCounterValues()
{
    std::cout << "commands sent     : " << std::dec
              << static_cast<int>(m_commandsSentCounter) << std::endl;
    std::cout << "valid responses   : "
              << static_cast<int>(m_validResponseCounter) << std::endl;
    std::cout << "invalid responses : "
              << static_cast<int>(m_inValidResponseCounter) << std::endl;
    std::cout << "bytes sent        : "
              << static_cast<int>(m_bytesSent) << std::endl;
    std::cout << "bytes received    : "
              << static_cast<int>(m_bytesReceived) << std::endl;
}

void monitor::printDebug(bool d)
{
    m_uart.printDebug(d);
    m_parser.printDebug(d);
}
