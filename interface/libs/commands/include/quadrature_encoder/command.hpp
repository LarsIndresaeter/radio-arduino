#pragma once

#include <common/uartCommandBase.hpp>

class UartCommandQuadratureEncoder : public UartCommandBase {
public:
    UartCommandQuadratureEncoder()
        : UartCommandBase(
            static_cast<uint8_t>(COMMANDS::OI::QUADRATURE_ENCODER),
            COMMANDS::QUADRATURE_ENCODER::COMMAND_LENGTH)
    {
        COMMANDS::QUADRATURE_ENCODER::command_t command;

    };

    void printResponse(std::ostream& out, COMMANDS::QUADRATURE_ENCODER::response_t response) const
    {
        out << "QUADRATURE_ENCODER   : ";

        if (response.status == 1) {
            uint16_t countPositive = response.getCountpositive();
            uint16_t countNegative = response.getCountnegative();
            uint16_t switchCount = response.getSwitchcount();

            out << "OK (";
            out << "countPositive=" << std::to_string(countPositive);
            out << " countNegative=" << std::to_string(countNegative);
            out << " value=" << std::to_string(countPositive - countNegative);
            out << " switchCount=" << std::to_string(switchCount);
            out << " switchPosition=" << std::to_string(response.getSwitchposition());
            out << ")";
        }
        else {
            out << "FAILED";
        }
    }

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::QUADRATURE_ENCODER::RESPONSE_LENGTH + 4)) {
            COMMANDS::QUADRATURE_ENCODER::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        } else
        {
            std::cout << "QUADRATURE_ENCODER: insufficient data" << std::endl;
        }
    };

    COMMANDS::QUADRATURE_ENCODER::response_t responseStruct()
    {
        return { (uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH] };
    };
};

