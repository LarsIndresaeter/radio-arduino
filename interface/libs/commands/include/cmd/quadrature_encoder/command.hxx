#pragma once
// This file is generated with the script: `interface/libs/commands/generate.py`

#include <common/raduinoCommandBase.hpp>

class RaduinoCommandQuadratureEncoder : public RaduinoCommandBase {
public:
    RaduinoCommandQuadratureEncoder()
        : RaduinoCommandBase(
              static_cast<uint8_t>(COMMANDS::OI::QUADRATURE_ENCODER),
              COMMANDS::QUADRATURE_ENCODER::COMMAND_LENGTH)
    {
        COMMANDS::QUADRATURE_ENCODER::command_t command;
    };

    void printResponse(std::ostream& out, COMMANDS::QUADRATURE_ENCODER::response_t response) const
    {
        out << "QUADRATURE_ENCODER     : ";
        out << " countnegative=" << static_cast<uint32_t>(response.getCountnegative());
        out << " countpositive=" << static_cast<uint32_t>(response.getCountpositive());
        out << " switchposition=" << static_cast<uint32_t>(response.getSwitchposition());
        out << " switchcount=" << static_cast<uint32_t>(response.getSwitchcount());
    }

    std::string getCommandName() { return "quadrature_encoder"; }

    std::string getJson()
    {
        std::string json;
        json.append("{");
        json.append("\"name\":");
        json.append("\"quadrature_encoder\", ");
        json.append("\"timestamp\":");
        json.append(std::to_string(getTimeStamp()));
        json.append(", ");
        json.append("\"countnegative\":");
        json.append(std::to_string(responseStruct().getCountnegative()));
        json.append(", ");
        json.append("\"countpositive\":");
        json.append(std::to_string(responseStruct().getCountpositive()));
        json.append(", ");
        json.append("\"switchposition\":");
        json.append(std::to_string(responseStruct().getSwitchposition()));
        json.append(", ");
        json.append("\"switchcount\":");
        json.append(std::to_string(responseStruct().getSwitchcount()));
        json.append("}");
        return (json);
    };

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::QUADRATURE_ENCODER::RESPONSE_LENGTH + 4)) {
            COMMANDS::QUADRATURE_ENCODER::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        }
        else {
            std::cout << "QUADRATURE_ENCODER: insufficient data" << std::endl;
        }
    };

    COMMANDS::QUADRATURE_ENCODER::response_t responseStruct()
    {
        COMMANDS::QUADRATURE_ENCODER::response_t response;

        if (m_responsePayload.size() >= sizeof(response)) {
            return { (uint8_t*)&m_responsePayload[0] };
        }

        return (response);
    };
};

