#pragma once
// This file is generated with the script: `interface/libs/commands/generate.py`

#include <common/raduinoCommandBase.hpp>

class RaduinoCommandPwm : public RaduinoCommandBase {
public:
    RaduinoCommandPwm(uint8_t port, uint8_t pin, uint8_t value)
        : RaduinoCommandBase(
              static_cast<uint8_t>(COMMANDS::OI::PWM),
              COMMANDS::PWM::COMMAND_LENGTH)
    {
        COMMANDS::PWM::command_t command;

        m_payload.at(offsetof(COMMANDS::PWM::command_t, port)) = port;

        m_payload.at(offsetof(COMMANDS::PWM::command_t, pin)) = pin;

        m_payload.at(offsetof(COMMANDS::PWM::command_t, value)) = value;
    };

    void printResponse(std::ostream& out, COMMANDS::PWM::response_t response) const
    {
        out << "PWM                    : ";
        out << " port=" << static_cast<uint32_t>(response.getPort());
        out << " pin=" << static_cast<uint32_t>(response.getPin());
        out << " value=" << static_cast<uint32_t>(response.getValue());
    }

    std::string getCommandName() { return "pwm"; }

    std::string getJson()
    {
        std::string json;
        json.append("{");
        json.append("\"name\":");
        json.append("\"pwm\", ");
        json.append("\"timestamp\":");
        json.append(std::to_string(getTimeStamp()));
        json.append(", ");
        json.append("\"port\":");
        json.append(std::to_string(responseStruct().getPort()));
        json.append(", ");
        json.append("\"pin\":");
        json.append(std::to_string(responseStruct().getPin()));
        json.append(", ");
        json.append("\"value\":");
        json.append(std::to_string(responseStruct().getValue()));
        json.append("}");
        return (json);
    };

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::PWM::RESPONSE_LENGTH + 4)) {
            COMMANDS::PWM::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        }
        else {
            std::cout << "PWM: insufficient data" << std::endl;
        }
    };

    COMMANDS::PWM::response_t responseStruct()
    {
        COMMANDS::PWM::response_t response;

        if (m_responsePayload.size() >= sizeof(response)) {
            return { (uint8_t*)&m_responsePayload[0] };
        }

        return (response);
    };
};

