#pragma once

#include <common/uartCommandBase.hpp>

class UartCommandIna219 : public UartCommandBase {
public:
    UartCommandIna219()

        : UartCommandBase(
            static_cast<uint8_t>(COMMANDS::OI::INA219),
            COMMANDS::INA219::COMMAND_LENGTH)
    {
    };

    void print(std::ostream& out) const override
    {
        COMMANDS::INA219::response_t response(
            (uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH]);

        int16_t voltage
            = ((uint16_t)response.voltage[0]) << 8 | response.voltage[1];
        voltage = voltage >> 3; // ignore 3 LSB
        float voltageFloat = voltage*0.004; // LSB = 4 mV


        int16_t current
            = ((uint16_t)response.current[0]) << 8 | response.current[1];
        float currentFloat = current*0.001;

        out << "INA219   : V=";
        out << voltageFloat << " ";
        out << " I=" << currentFloat;

        //if (0 == response.status) {
            //out << "OK";
        //}
        //else if (1 == response.status) {
            //out << "nack";
        //}
        //else if (2 == response.status) {
            //out << "transmission failure";
        //}
    };

    COMMANDS::INA219::response_t responseStruct()

    {
        return { (uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH] };
    };
};

