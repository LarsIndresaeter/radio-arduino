#pragma once

#include <chrono>
#include <cmd/commands.hxx>
#include <cmd/payloads.hxx>
#include <condition_variable>
#include <eventprocess.hpp>
#include <map>
#include <mutex>
#include <protocol.hpp>
#include <uart.hpp>
#include <vector>

using ProtocolCallback = std::function<void(std::vector<uint8_t>&)>;

using namespace std::chrono_literals;

constexpr std::chrono::milliseconds default_timeout = 750ms;

class monitor {
public:
    monitor(Uart& uart, EventProcess& ep, CryptoHandlerInterface* cryptoHandler);
    void printCounterValues();
    void printDebug(bool d);
    void setPrintResponseTime(bool value);
    void setTransportEncryption(bool value);
    int getCommandsSent();
    int getValidResponses();
    int getInvalidResponses();
    int getBytesSent();
    int getBytesReceived();
    bool lastCommandReturnedValidResponse();

    template <typename T> T get(T cmd)
    {
        if (m_transportEncryption) {
            return get(cmd, static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::GATEWAY_ENCRYPTED), default_timeout);
        }
        else {
            return get(cmd, static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::GATEWAY), default_timeout);
        }
    }

    template <typename T> T get(T cmd, std::chrono::milliseconds timeout)
    {
        if (m_transportEncryption) {
            return get(cmd, static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::GATEWAY_ENCRYPTED), timeout);
        }
        else {
            return get(cmd, static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::GATEWAY), timeout);
        }
    }

    template <typename T> T getRadio(T cmd)
    {
        if (m_transportEncryption) {
            return get(cmd, static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::NODE_ENCRYPTED), default_timeout);
        }
        else {
            return get(cmd, static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::NODE), default_timeout);
        }
    }

    template <typename T> T getRadio(T cmd, std::chrono::milliseconds timeout)
    {
        if (m_transportEncryption) {
            return get(cmd, static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::NODE_ENCRYPTED), timeout);
        }
        else {
            return get(cmd, static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::NODE), timeout);
        }
    }

    template <typename T> T get(T cmd, uint8_t protocol_version, std::chrono::milliseconds timeout)
    {
        cmd.setReplyStatus(RaduinoCommandBase::ReplyStatus::Pending);
        m_lastResponseValid = false;

        auto start = std::chrono::high_resolution_clock::now();

        get(cmd.getPayload(), protocol_version);

        setResponseCallback(cmd.getCommandId(), [&cmd](std::vector<uint8_t> data) { cmd.setResponse(data); });

        std::unique_lock<std::mutex> lock(m_mutex);

        if (!m_cv.wait_for(lock, timeout, [&cmd] { return (cmd.getResponse().size() > 0); })) {
            // std::cout << "Reply timeout" << std::endl;
            cmd.setReplyStatus(RaduinoCommandBase::ReplyStatus::Timeout);
            m_inValidResponseCounter++;

            // we don't want a late response calling setResponse on an object out of scope
            eraseResponseCallback(cmd.getCommandId());

            return (cmd);
        }

        auto end = std::chrono::high_resolution_clock::now();

        auto dur = end - start;
        auto i_millis = std::chrono::duration_cast<std::chrono::microseconds>(dur);
        uint64_t time_since_epoch_ms
            = std::chrono::duration_cast<std::chrono::milliseconds>(start.time_since_epoch()).count();
        cmd.setTimeStamp(time_since_epoch_ms);
        cmd.setResponseTimeUs(i_millis.count());
        if (m_printResponseTime) {
            std::cout << "[" << std::dec << static_cast<int>(cmd.getResponseTimeUs()) << "]";
        }

        cmd.validateResponse(); // set status to Error if response is not valid

        if (cmd.getReplyStatus() == RaduinoCommandBase::ReplyStatus::Complete) {
            m_lastResponseValid = true;
        }
        else {
            m_lastResponseValid = false;
        }

        return (cmd);
    }

private:
    void get(std::vector<uint8_t> command, uint8_t protocol_version);
    void setResponseCallback(uint8_t cmd, ProtocolCallback cb);
    void eraseResponseCallback(uint8_t cmd);
    void sendRequest(COMMANDS::OI cmd, std::vector<uint8_t> data, uint8_t protocol_version);
    const void reply(std::vector<uint8_t> data);
    Uart& m_uart;
    EventProcess& m_ep;
    LinuxProtocol m_parser;
    std::map<uint8_t, ProtocolCallback> m_responseCallback;
    std::condition_variable m_cv;
    std::mutex m_mutex;
    uint32_t m_validResponseCounter;
    uint32_t m_inValidResponseCounter;
    bool m_lastResponseValid;
    uint32_t m_commandsSentCounter;
    uint32_t m_bytesSent;
    uint32_t m_bytesReceived;
    std::string m_receivedString;
    bool m_printResponseTime = false;
    bool m_transportEncryption = false;
    bool m_printDebug = false;
};
