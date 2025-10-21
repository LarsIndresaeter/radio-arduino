#include <ina219.hpp>

// resisters
// CONFIGURATION = 0x00,
// SHUNT_VOLTAGE = 0x01,
// BUS_VOLTAGE = 0x02,
// POWER = 0x03,
// CURRENT = 0x04,
// CALIBRATION = 0x05,

#define range_40_mV 0x00
#define range_80_mV 0x01
#define range_160_mv 0x10
#define range_320_mV 0x11 // 3.2A range with 0.1 ohm resistor

#define samples_1_9bit 0x0011  // 84 uS conversion time
#define samples_1_12bit 0x0011 // 532 uS conversion time
#define samples_1 0x1000
#define samples_2 0x1001
#define samples_4 0x1010
#define samples_8 0x1011
#define samples_16 0x1100
#define samples_32 0x1101
#define samples_64 0x1110
#define samples_128 0x1111

#define mode_power_down 0x000
#define mode_shunt_voltage_triggered 0x001
#define mode_bus_voltage_triggered 0x010
#define mode_shunt_and_bus_triggered 0x011
#define mode_adc_off 0x100
#define mode_shunt_voltage_continuous 0x101
#define mode_bus_voltage_continuous 0x110
#define mode_shunt_and_bus_continuous 0x111

void configureIna219(uint8_t address)
{
    // set configuration register
    uint16_t configurationValue = range_40_mV << 11;
    configurationValue |= samples_2 << 3;
    configurationValue |= mode_shunt_and_bus_triggered << 0;
    I2C_Init();
    I2C_Start(address);
    I2C_Write(0x00); // register address
    I2C_Write(configurationValue >> 8);
    I2C_Write(configurationValue);
    I2C_Stop();

    // set calibration value
    uint16_t calibrationValue = 2048;
    I2C_Init();
    I2C_Start(address);
    I2C_Write(0x05); // register address
    I2C_Write(calibrationValue >> 8);
    I2C_Write(calibrationValue);
    I2C_Stop();
}

bool ina219NotConfigured = true;

uint16_t readIna219Voltage(void)
{
    uint8_t address = 0x80;
    uint16_t voltage;

    if (ina219NotConfigured) {
        configureIna219(address);
        ina219NotConfigured = false;
    }

    // set register pointer to voltage register
    I2C_Init();
    I2C_Start(address);
    I2C_Write(2); // voltage
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
    uint8_t address = 0x80;
    uint16_t current;

    if (ina219NotConfigured) {
        configureIna219(address);
        ina219NotConfigured = false;
    }

    // set register pointer to current register
    I2C_Init();
    I2C_Start(address);
    I2C_Write(4); // current
    I2C_Stop();

    I2C_Init();
    I2C_Start(address + 1);
    current = (uint16_t)I2C_Read_Ack() << 8;
    current |= ((uint16_t)I2C_Read_Nack());
    I2C_Stop();

    return current;
}
