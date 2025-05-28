#pragma once

#include <common/uartCommandBase.hpp>

class UartCommandQuadratureEncoder : public UartCommandBase {
public:
    UartCommandQuadratureEncoder()
        : UartCommandBase(
            static_cast<uint8_t>(COMMANDS::OI::QUADRATURE_ENCODER),
            COMMANDS::QUADRATURE_ENCODER::COMMAND_LENGTH)
    {
    };

    void print(std::ostream& out) const override
    {
        COMMANDS::QUADRATURE_ENCODER::response_t response(
            (uint8_t*)&m_response.data()[4]);
        out << "QUADRATURE_ENCODER : ";

        if(response.status == 1)
        {
            uint16_t cnt_pos = (response.cnt_pos_high<<8) + response.cnt_pos_low;
            uint16_t cnt_neg = (response.cnt_neg_high<<8) + response.cnt_neg_low;

            out << "OK (";
            out << "pos=" << std::to_string(cnt_pos) << " neg=" << std::to_string(cnt_neg) << ")";
        }
        else
        {
            out << "FAILED";
        }
    };

    COMMANDS::QUADRATURE_ENCODER::response_t responseStruct()
    {
        return {(uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH]};
    };
};

