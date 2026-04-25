#pragma once
// This file is generated with the script: `interface/libs/commands/generate.py`

#include <common/raduinoCommandBase.hpp>

class RaduinoCommandScanForAdvertisement : public RaduinoCommandBase {
public:
    RaduinoCommandScanForAdvertisement(uint32_t id, uint16_t timeout)
        : RaduinoCommandBase(
              static_cast<uint8_t>(COMMANDS::OI::SCAN_FOR_ADVERTISEMENT),
              COMMANDS::SCAN_FOR_ADVERTISEMENT::COMMAND_LENGTH)
    {
        COMMANDS::SCAN_FOR_ADVERTISEMENT::command_t command;

        m_payload.at(offsetof(COMMANDS::SCAN_FOR_ADVERTISEMENT::command_t, id) + 3) = id>>24;
        m_payload.at(offsetof(COMMANDS::SCAN_FOR_ADVERTISEMENT::command_t, id) + 2) = id>>16;
        m_payload.at(offsetof(COMMANDS::SCAN_FOR_ADVERTISEMENT::command_t, id) + 1) = id>>8;
        m_payload.at(offsetof(COMMANDS::SCAN_FOR_ADVERTISEMENT::command_t, id)) = id;

        m_payload.at(offsetof(COMMANDS::SCAN_FOR_ADVERTISEMENT::command_t, timeout) + 1) = timeout>>8;
        m_payload.at(offsetof(COMMANDS::SCAN_FOR_ADVERTISEMENT::command_t, timeout)) = timeout;
    };

    void printResponse(std::ostream& out, COMMANDS::SCAN_FOR_ADVERTISEMENT::response_t response) const
    {
        out << "SCAN_FOR_ADVERTISEMENT   : ";
        out << " sequence_number=" << static_cast<uint32_t>(response.getSequence_number());
        out << " data=[ ";
        out << std::setfill('0') << std::hex << std::uppercase;
        for (uint8_t i = 0; i < 16; i++) {
            out << std::setw(2) << static_cast<uint32_t>(response.data[i]) << " ";
        }
        out << "]";
        out << std::dec;
        out << " flags=" << static_cast<uint32_t>(response.getFlags());
        out << " id=" << static_cast<uint32_t>(response.getId());
    }

    std::string getData()
    {
        std::string retval;
        COMMANDS::SCAN_FOR_ADVERTISEMENT::response_t response = responseStruct();

        retval.append("[");
        for (uint8_t i = 0; i < 16; i++) {
            retval.append(" \"");
            retval.append(std::to_string(static_cast<uint32_t>(response.data[i])));
            if (i < (16 - 1)) {
                retval.append("\",");
            }
            else {
                retval.append("\"");
            }
        }
        retval.append(" ]");

        return (retval);
    }
    std::string getCommandName() { return "scan_for_advertisement"; }

    std::string getJson()
    {
        std::string json;
        json.append("{");
        json.append("\"name\":\"" + getCommandName() + "\", ");
        json.append(getJsonCommonFields());
        json.append("\"payload\":{");
        json.append("\"sequence_number\":");
        json.append(std::to_string(responseStruct().getSequence_number()));
        json.append(", ");
        json.append("\"data\": ");
        json.append("\"" + getData() + "\"");
        json.append("");
        json.append(", ");
        json.append("\"flags\":");
        json.append(std::to_string(responseStruct().getFlags()));
        json.append(", ");
        json.append("\"id\":");
        json.append(std::to_string(responseStruct().getId()));
        json.append("}}");
        return (json);
    };

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::SCAN_FOR_ADVERTISEMENT::RESPONSE_LENGTH + 4)) {
            COMMANDS::SCAN_FOR_ADVERTISEMENT::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        }
        else {
            std::cout << "SCAN_FOR_ADVERTISEMENT: insufficient data" << std::endl;
        }
    };

    COMMANDS::SCAN_FOR_ADVERTISEMENT::response_t responseStruct()
    {
        COMMANDS::SCAN_FOR_ADVERTISEMENT::response_t response;

        if (m_responsePayload.size() >= sizeof(response)) {
            return { (uint8_t*)&m_responsePayload[0] };
        }

        return (response);
    };
};

