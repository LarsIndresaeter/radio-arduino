#pragma once
// This file is generated with the script: `interface/libs/commands/generate.py`

#include <common/raduinoCommandBase.hpp>

class RaduinoCommandIna219 : public RaduinoCommandBase {
public:
    RaduinoCommandIna219()
        : RaduinoCommandBase(
              static_cast<uint8_t>(COMMANDS::OI::INA219),
              COMMANDS::INA219::COMMAND_LENGTH)
    {
        COMMANDS::INA219::command_t command;
    };

    void printResponse(std::ostream& out, COMMANDS::INA219::response_t response) const
    {
        out << "INA219                 : ";
        out << " current=" << static_cast<int>(response.getCurrent());
        out << " voltage=" << static_cast<int>(response.getVoltage());
    }

    std::string getCommandName() { return "ina219"; }

    std::string getJson()
    {
        std::string json;
        json.append("{");
        json.append("\"name\":");
        json.append("\"ina219\", ");
        json.append("\"timestamp\":");
        json.append(std::to_string(getTimeStamp()));
        json.append(", ");
        json.append("\"current\":");
        json.append(std::to_string(responseStruct().getCurrent()));
        json.append(", ");
        json.append("\"voltage\":");
        json.append(std::to_string(responseStruct().getVoltage()));
        json.append("}");
        return (json);
    };

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::INA219::RESPONSE_LENGTH + 4)) {
            COMMANDS::INA219::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        }
        else {
            std::cout << "INA219: insufficient data" << std::endl;
        }
    };

    COMMANDS::INA219::response_t responseStruct()
    {
        COMMANDS::INA219::response_t response;

        if (m_responsePayload.size() >= sizeof(response)) {
            return { (uint8_t*)&m_responsePayload[0] };
        }

        return (response);
    };
};

