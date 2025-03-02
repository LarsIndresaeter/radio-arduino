#pragma once

#include <chrono>
#include <commands.hpp>
#include <condition_variable>
#include <eventprocess.hpp>
#include <map>
#include <mutex>
#include <payloads.hpp>
#include <protocol.hpp>
#include <uart.hpp>
#include <vector>

using ProtocolCallback = std::function<void(std::vector<uint8_t>&)>;

using namespace std::chrono_literals;

constexpr std::chrono::milliseconds default_timeout = 2000ms;

class monitor {
public:
    monitor(
        Uart& uart, EventProcess& ep, CryptoHandlerInterface* cryptoHandler);
    void printCounterValues();
    void printDebug(bool d);
    void setPrintResponseTime(bool value);
    void setTransportEncryption(bool value);

    template <typename T> T get(T cmd)
    {
        if (m_transportEncryption) {
            return get(
                cmd,
                static_cast<uint8_t>(
                    PROTOCOL::HEADER::VERSION::ENCRYPTED_BINARY_AND_TEXT), default_timeout);
        }
        else {
            return get(
                cmd,
                static_cast<uint8_t>(
                    PROTOCOL::HEADER::VERSION::BINARY_AND_TEXT), default_timeout);
        }
    }

    template <typename T> T get(T cmd, std::chrono::milliseconds timeout)
    {
        if (m_transportEncryption) {
            return get(
                cmd,
                static_cast<uint8_t>(
                    PROTOCOL::HEADER::VERSION::ENCRYPTED_BINARY_AND_TEXT), timeout);
        }
        else {
            return get(
                cmd,
                static_cast<uint8_t>(
                    PROTOCOL::HEADER::VERSION::BINARY_AND_TEXT), timeout);
        }
    }

    template <typename T> T getRadio(T cmd)
    {
        if (m_transportEncryption) {
            return get(
                cmd,
                static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::
                                         RADIO_ENCRYPTED_BINARY_AND_TEXT), default_timeout);
        }
        else {
            return get(
                cmd,
                static_cast<uint8_t>(
                    PROTOCOL::HEADER::VERSION::RADIO_BINARY_AND_TEXT), default_timeout);
        }
    }

    template <typename T> T getRadio(T cmd, std::chrono::milliseconds timeout)
    {
        if (m_transportEncryption) {
            return get(
                cmd,
                static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::
                                         RADIO_ENCRYPTED_BINARY_AND_TEXT), timeout);
        }
        else {
            return get(
                cmd,
                static_cast<uint8_t>(
                    PROTOCOL::HEADER::VERSION::RADIO_BINARY_AND_TEXT), timeout);
        }
    }

    template <typename T> T get(T cmd, uint8_t protocol_version, std::chrono::milliseconds timeout)
    {
        cmd.setReplyStatus(UartCommandBase::ReplyStatus::Pending);

        auto start = std::chrono::high_resolution_clock::now();

        get(cmd.getPayload(), protocol_version);

        setResponseCallback(
            cmd.getCommandId(),
            [&cmd](std::vector<uint8_t> data) { cmd.setResponse(data); });

        std::unique_lock<std::mutex> lock(m_mutex);

        if (!m_cv.wait_for(lock, timeout, [&cmd] {
                return (cmd.getResponse().size() > 0);
            })) {
            // std::cout << "Reply timeout" << std::endl;
            cmd.setReplyStatus(UartCommandBase::ReplyStatus::Timeout);
            m_inValidResponseCounter++;
            return (cmd);
        }

        auto end = std::chrono::high_resolution_clock::now();

        auto dur = end - start;
        auto i_millis
            = std::chrono::duration_cast<std::chrono::microseconds>(dur);
        cmd.setResponseTimeUs(i_millis.count());
        if (m_printResponseTime) {
            std::cout << "[" << std::dec
                      << static_cast<int>(cmd.getResponseTimeUs()) << "]";
        }

        cmd.setReplyStatus(UartCommandBase::ReplyStatus::Complete);
        cmd.validateResponse(); // set status to Error if response is not valid

        return (cmd);
    }

private:
    void get(std::vector<uint8_t> command, uint8_t protocol_version);
    void setResponseCallback(uint8_t cmd, ProtocolCallback cb);
    void sendRequest(
        COMMANDS::OI cmd, std::vector<uint8_t> data, uint8_t protocol_version);
    const void reply(std::vector<uint8_t> data);
    Uart& m_uart;
    EventProcess& m_ep;
    LinuxProtocol m_parser;
    std::map<uint8_t, ProtocolCallback> m_responseCallback;
    std::condition_variable m_cv;
    std::mutex m_mutex;
    uint32_t m_validResponseCounter;
    uint32_t m_inValidResponseCounter;
    uint32_t m_commandsSentCounter;
    uint32_t m_bytesSent;
    uint32_t m_bytesReceived;
    std::string m_receivedString;
    bool m_printResponseTime = false;
    bool m_transportEncryption = false;
};

