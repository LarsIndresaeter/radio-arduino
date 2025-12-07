#pragma once
// This file is generated with the script: `interface/libs/commands/generate.py`

#include <common/raduinoCommandBase.hpp>

class RaduinoCommandGetLsm303d : public RaduinoCommandBase {
public:
    RaduinoCommandGetLsm303d()
        : RaduinoCommandBase(
              static_cast<uint8_t>(COMMANDS::OI::GET_LSM303D),
              COMMANDS::GET_LSM303D::COMMAND_LENGTH)
    {
        COMMANDS::GET_LSM303D::command_t command;
    };

    void printResponse(std::ostream& out, COMMANDS::GET_LSM303D::response_t response) const
    {
        out << "GET_LSM303D            : ";
        out << " accelerometerX=" << static_cast<uint32_t>(response.getAccelerometerx());
        out << " accelerometerY=" << static_cast<uint32_t>(response.getAccelerometery());
        out << " accelerometerZ=" << static_cast<uint32_t>(response.getAccelerometerz());
    }

    std::string getCommandName() { return "get_lsm303d"; }

    std::string getJson()
    {
        std::string json;
        json.append("{");
        json.append("\"name\":\"" + getCommandName() + "\", ");
        json.append(getJsonCommonFields());
        json.append("\"payload\":{");
        json.append("\"accelerometerX\":");
        json.append(std::to_string(responseStruct().getAccelerometerx()));
        json.append(", ");
        json.append("\"accelerometerY\":");
        json.append(std::to_string(responseStruct().getAccelerometery()));
        json.append(", ");
        json.append("\"accelerometerZ\":");
        json.append(std::to_string(responseStruct().getAccelerometerz()));
        json.append("}}");
        return (json);
    };

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::GET_LSM303D::RESPONSE_LENGTH + 4)) {
            COMMANDS::GET_LSM303D::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        }
        else {
            std::cout << "GET_LSM303D: insufficient data" << std::endl;
        }
    };

    COMMANDS::GET_LSM303D::response_t responseStruct()
    {
        COMMANDS::GET_LSM303D::response_t response;

        if (m_responsePayload.size() >= sizeof(response)) {
            return { (uint8_t*)&m_responsePayload[0] };
        }

        return (response);
    };
};

