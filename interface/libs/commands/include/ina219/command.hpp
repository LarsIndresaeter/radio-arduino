#pragma once

#include <common/uartCommandBase.hpp>

class UartCommandIna219 : public UartCommandBase {
public:
    UartCommandIna219()
        : UartCommandBase(
            static_cast<uint8_t>(COMMANDS::OI::INA219),
            COMMANDS::INA219::COMMAND_LENGTH)
    {
        COMMANDS::INA219::command_t command;

    };

    void printResponse(std::ostream& out, COMMANDS::INA219::response_t response) const
    {
        out << "INA219   : ";

        int16_t voltage = response.getVoltage();
        voltage = voltage >> 3; // ignore 3 LSB
        float voltageFloat = voltage*0.004; // LSB = 4 mV

        int16_t current = response.getCurrent();
        float currentFloat = current*0.001;

        out << "INA219   : V=";
        out << voltageFloat << " ";
        out << " I=" << currentFloat;
    };

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::INA219::RESPONSE_LENGTH + 4)) {
            COMMANDS::INA219::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        } else
        {
            std::cout << "INA219: insufficient data" << std::endl;
        }
    };

    COMMANDS::INA219::response_t responseStruct()
    {
        return { (uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH] };
    };
};

