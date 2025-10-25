#pragma once

#include <i2c.hpp>
#include <stdint.h>

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

#define ina219_register_configuration 0
#define ina219_register_shunt 1
#define ina219_register_bus 2
#define ina219_register_power 3
#define ina219_register_current 4
#define ina219_register_calibration 5

uint16_t readIna219Voltage(void);
uint16_t readIna219Current(void);
