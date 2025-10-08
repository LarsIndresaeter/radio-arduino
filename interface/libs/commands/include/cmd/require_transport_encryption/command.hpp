#pragma once
// This file is generated with the script: `interface/libs/commands/generate.py`

#include <common/uartCommandBase.hpp>

class UartCommandRequireTransportEncryption : public UartCommandBase {
public:
    UartCommandRequireTransportEncryption(uint8_t value, uint8_t persist)
        : UartCommandBase(
              static_cast<uint8_t>(COMMANDS::OI::REQUIRE_TRANSPORT_ENCRYPTION),
              COMMANDS::REQUIRE_TRANSPORT_ENCRYPTION::COMMAND_LENGTH)
    {
        COMMANDS::REQUIRE_TRANSPORT_ENCRYPTION::command_t command;

        m_payload.at(offsetof(COMMANDS::REQUIRE_TRANSPORT_ENCRYPTION::command_t, value)) = value;

        m_payload.at(offsetof(COMMANDS::REQUIRE_TRANSPORT_ENCRYPTION::command_t, persist)) = persist;
    };

    void printResponse(std::ostream& out, COMMANDS::REQUIRE_TRANSPORT_ENCRYPTION::response_t response) const
    {
        out << "REQUIRE_TRANSPORT_ENCRYPTION   : ";
    }

    std::string getCommandName() { return "require_transport_encryption"; }

    std::string getJson()
    {
        std::string json;
        json.append("{");
        json.append("\"name\":");
        json.append("\"require_transport_encryption\", ");
        json.append("\"timestamp\":");
        json.append(std::to_string(getTimeStamp()));
        json.append(", ");
        json.append("}");
        return (json);
    };

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::REQUIRE_TRANSPORT_ENCRYPTION::RESPONSE_LENGTH + 4)) {
            COMMANDS::REQUIRE_TRANSPORT_ENCRYPTION::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        }
        else {
            std::cout << "REQUIRE_TRANSPORT_ENCRYPTION: insufficient data" << std::endl;
        }
    };

    COMMANDS::REQUIRE_TRANSPORT_ENCRYPTION::response_t responseStruct()
    {
        COMMANDS::REQUIRE_TRANSPORT_ENCRYPTION::response_t response;

        if (m_responsePayload.size() >= sizeof(response)) {
            return { (uint8_t*)&m_responsePayload[0] };
        }

        return (response);
    };
};

