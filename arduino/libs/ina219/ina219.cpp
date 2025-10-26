#include "include/ina219.hpp"
#include <ina219.hpp>

void configureIna219(uint8_t address)
{
    // set configuration register
    uint16_t configurationValue = range_160_mV << 12; // 1.6A
    configurationValue |= samples_4 << 3; // 2.13 ms
    configurationValue |= mode_shunt_and_bus_triggered << 0;
    I2C_Init();
    I2C_Start(address);
    I2C_Write(ina219_register_configuration);
    I2C_Write(configurationValue >> 8);
    I2C_Write(configurationValue);
    I2C_Stop();

    // set calibration value
    uint16_t calibrationValue = 2048;
    I2C_Init();
    I2C_Start(address);
    I2C_Write(ina219_register_calibration);
    I2C_Write(calibrationValue >> 8);
    I2C_Write(calibrationValue);
    I2C_Stop();
}

bool ina219NotConfigured = true;

uint16_t readIna219Voltage(void)
{
    uint8_t address = device_address_ina219;
    uint16_t voltage;

    if (ina219NotConfigured) {
        configureIna219(address);
        ina219NotConfigured = false;
    }

    // set register pointer to voltage register
    I2C_Init();
    I2C_Start(address);
    I2C_Write(ina219_register_bus);
    I2C_Stop();

    I2C_Init();
    I2C_Start(address + 1);
    voltage = (uint16_t)I2C_Read_Ack() << 8;
    voltage |= ((uint16_t)I2C_Read_Nack());
    I2C_Stop();

    return voltage;
}

uint16_t readIna219Current(void)
{
    uint8_t address = device_address_ina219;
    uint16_t current;

    if (ina219NotConfigured) {
        configureIna219(address);
        ina219NotConfigured = false;
    }

    // set register pointer to current register
    I2C_Init();
    I2C_Start(address);
    I2C_Write(ina219_register_current);
    I2C_Stop();

    I2C_Init();
    I2C_Start(address + 1);
    current = (uint16_t)I2C_Read_Ack() << 8;
    current |= ((uint16_t)I2C_Read_Nack());
    I2C_Stop();

    return current;
}
