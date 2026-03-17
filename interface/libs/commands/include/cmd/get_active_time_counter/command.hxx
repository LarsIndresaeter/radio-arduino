#pragma once
// This file is generated with the script: `interface/libs/commands/generate.py`

#include <common/raduinoCommandBase.hpp>

class RaduinoCommandGetActiveTimeCounter : public RaduinoCommandBase {
public:
    RaduinoCommandGetActiveTimeCounter(std::vector<uint8_t> timestamp)
        : RaduinoCommandBase(
              static_cast<uint8_t>(COMMANDS::OI::GET_ACTIVE_TIME_COUNTER),
              COMMANDS::GET_ACTIVE_TIME_COUNTER::COMMAND_LENGTH)
    {
        COMMANDS::GET_ACTIVE_TIME_COUNTER::command_t command;

        for (uint8_t i = 0; i < sizeof(command.timestamp); i++) {
            if (i >= timestamp.size()) {
                break;
            }
            m_payload.at(
                offsetof(COMMANDS::GET_ACTIVE_TIME_COUNTER::command_t, timestamp[0]) + i)
                = timestamp.at(i);
        }
    };

    // string constructor
    RaduinoCommandGetActiveTimeCounter(std::string timestamp)
        : RaduinoCommandBase(
              static_cast<uint8_t>(COMMANDS::OI::GET_ACTIVE_TIME_COUNTER),
              COMMANDS::GET_ACTIVE_TIME_COUNTER::COMMAND_LENGTH)
    {
        COMMANDS::GET_ACTIVE_TIME_COUNTER::command_t command;

        for (uint8_t i = 0; i < timestamp.size() & i < 8; i++) {
            m_payload.at(
                offsetof(COMMANDS::GET_ACTIVE_TIME_COUNTER::command_t, timestamp[0]) + i)
                = timestamp.at(i);
        }
    };

    void printResponse(std::ostream& out, COMMANDS::GET_ACTIVE_TIME_COUNTER::response_t response) const
    {
        out << "GET_ACTIVE_TIME_COUNTER   : ";
        out << " active_time=[ ";
        out << std::setfill('0') << std::hex << std::uppercase;
        for (uint8_t i = 0; i < 8; i++) {
            out << std::setw(2) << static_cast<uint32_t>(response.active_time[i]) << " ";
        }
        out << "]";
        out << std::dec;
        out << " sleep_time=[ ";
        out << std::setfill('0') << std::hex << std::uppercase;
        for (uint8_t i = 0; i < 8; i++) {
            out << std::setw(2) << static_cast<uint32_t>(response.sleep_time[i]) << " ";
        }
        out << "]";
        out << std::dec;
    }

    std::string getActive_time()
    {
        std::string retval;
        COMMANDS::GET_ACTIVE_TIME_COUNTER::response_t response = responseStruct();

        retval.append("[");
        for (uint8_t i = 0; i < 8; i++) {
            retval.append(" \"");
            retval.append(std::to_string(static_cast<uint32_t>(response.active_time[i])));
            if (i < (8 - 1)) {
                retval.append("\",");
            }
            else {
                retval.append("\"");
            }
        }
        retval.append(" ]");

        return (retval);
    }
    std::string getSleep_time()
    {
        std::string retval;
        COMMANDS::GET_ACTIVE_TIME_COUNTER::response_t response = responseStruct();

        retval.append("[");
        for (uint8_t i = 0; i < 8; i++) {
            retval.append(" \"");
            retval.append(std::to_string(static_cast<uint32_t>(response.sleep_time[i])));
            if (i < (8 - 1)) {
                retval.append("\",");
            }
            else {
                retval.append("\"");
            }
        }
        retval.append(" ]");

        return (retval);
    }
    std::string getCommandName() { return "get_active_time_counter"; }

    std::string getJson()
    {
        std::string json;
        json.append("{");
        json.append("\"name\":\"" + getCommandName() + "\", ");
        json.append(getJsonCommonFields());
        json.append("\"payload\":{");
        json.append("\"active_time\":");
        json.append(std::to_string(responseStruct().getActive_time()));
        json.append(", ");
        json.append("\"sleep_time\":");
        json.append(std::to_string(responseStruct().getSleep_time()));
        json.append("}}");
        return (json);
    };

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::GET_ACTIVE_TIME_COUNTER::RESPONSE_LENGTH + 4)) {
            COMMANDS::GET_ACTIVE_TIME_COUNTER::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        }
        else {
            std::cout << "GET_ACTIVE_TIME_COUNTER: insufficient data" << std::endl;
        }
    };

    COMMANDS::GET_ACTIVE_TIME_COUNTER::response_t responseStruct()
    {
        COMMANDS::GET_ACTIVE_TIME_COUNTER::response_t response;

        if (m_responsePayload.size() >= sizeof(response)) {
            return { (uint8_t*)&m_responsePayload[0] };
        }

        return (response);
    };
};

