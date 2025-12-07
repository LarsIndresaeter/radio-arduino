#include "include/lsm303d.hpp"
#include <i2c.hpp>

bool lsm303dConfigured = false;

void writeRegister(uint8_t registerAddress, uint8_t registerValue)
{
    I2C_Init();
    I2C_Start(device_address_lsm303d);
    I2C_Write(registerAddress);
    I2C_Write(registerValue);
    I2C_Stop();
}
void configureLsm303d()
{
    if (lsm303dConfigured == false) {
        writeRegister(lsm303d_register_ctrl1, lsm303d_ctrl1_value); // enable accelerometer
        lsm303dConfigured = true;
    }
}

uint8_t readRegister(uint8_t registerAddress)
{
    uint8_t result;

    I2C_Init();
    I2C_Start(device_address_lsm303d);
    I2C_Write(registerAddress);
    I2C_Stop();

    I2C_Init();
    I2C_Start(device_address_lsm303d + 1);
    result = I2C_Read_Nack();
    I2C_Stop();

    return (result);
}

uint16_t lsm303dReadDoubleRegister(uint8_t registerAddress)
{
    uint16_t result;

    configureLsm303d();

    result = readRegister(registerAddress); // low value register
    result |= ((uint16_t)readRegister(registerAddress + 1)) << 8;

    return result;
}


