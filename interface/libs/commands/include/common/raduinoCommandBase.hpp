#pragma once

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <cmd/payloads.hpp>
#include <protocolBase.hpp>
#include <stdint.h>
#include <vector>

class RaduinoCommandBase {
public:
    enum class ReplyStatus
    {
        Pending,
        Complete,
        Error,
        Timeout
    };

    RaduinoCommandBase(uint8_t cmd, uint8_t length)
        : m_cmdId(cmd)
    {
        m_payload.push_back(cmd);
        m_payload.push_back(length);
        for (int i = 0; i < length; i++) {
            m_payload.push_back(0);
        }
    };

    RaduinoCommandBase(std::vector<uint8_t> payload);

    std::vector<uint8_t> getResponse() { return (m_response); };

    virtual std::vector<uint8_t> getPayload() const { return (m_payload); };

    virtual void setResponse(const std::vector<uint8_t> response)
    {
        m_response.insert(m_response.end(), response.begin(), response.end());

        if (response.size() > 4) {
            std::copy(response.begin() + 4, response.end(),
                std::back_inserter(m_responsePayload));
        }
    };

    void setReplyStatus(ReplyStatus status)
    {
        m_replyStatus = status;
    };

    ReplyStatus getReplyStatus() { return (m_replyStatus); };

    virtual void validateResponse()
    {
        m_replyStatus = ReplyStatus::Complete;

        // check if resonse vector size matches object length
        if (m_response.size() < PROTOCOL::HEADER::LENGTH
                + m_response.at(PROTOCOL::PAYLOAD::OL_OFFSET)
                + PROTOCOL::CHECKSUM::LENGTH) {
            m_replyStatus = ReplyStatus::Error;
            return;
        }

        // check if response message has the correct object id
        if (m_response.at(PROTOCOL::PAYLOAD::PAYLOAD_OFFSET) != m_cmdId) {
            m_replyStatus = ReplyStatus::Error;
            return;
        }

        // check if packet length and object length matches
        if (m_response.at(PROTOCOL::HEADER::LENGTH_OFFSET)
            != (m_response.at(PROTOCOL::PAYLOAD::OL_OFFSET) + 2)) {
            m_replyStatus = ReplyStatus::Error;
            return;
        }
    };

    void setResponseTimeUs(uint64_t duration) { m_responseTimeUs = duration; };

    uint64_t getResponseTimeUs() { return (m_responseTimeUs); };

    uint8_t getCommandId() { return (m_cmdId); };

    void setTimeStamp(uint64_t timestamp) { m_timeStamp = timestamp;};
    uint64_t getTimeStamp() { return (m_timeStamp); };

    std::string getCommandName() { return ("UNDEFINED"); };

    std::string getJson() { return ("{}"); };

    friend std::ostream& operator<<(std::ostream& out, RaduinoCommandBase const& u)
    {
        uint8_t c;
        if (u.m_replyStatus == ReplyStatus::Timeout) {
            out << "timeout  : ";
        }
        else if (u.m_replyStatus == ReplyStatus::Error) {
            out << "error    : ";
        }
        else if (u.m_replyStatus == ReplyStatus::Pending) {
            out << "pending  : ";
        }
        else if (u.m_replyStatus == ReplyStatus::Complete) {
            out << "complete : ";
        }
 
        if (u.m_replyStatus == ReplyStatus::Complete) {
            u.print(out, u.m_responsePayload);
        }

        return out;
    };

protected:
    virtual void print(std::ostream& out) const
    {
   }

    virtual void print(std::ostream& out, std::vector<uint8_t> responsePayload) const
    {
    };

    uint64_t m_timeStamp;
    uint64_t m_responseTimeUs;
    ReplyStatus m_replyStatus;
    std::vector<uint8_t> m_payload;
    std::vector<uint8_t> m_responsePayload;
    std::vector<uint8_t> m_response;
    uint8_t m_cmdId;
};

