#pragma once

#include <i2c.hpp>
#include <stdint.h>

#define device_address_lsm303d 50

#define lsm303d_register_ctrl1 0x20

#define lsm303d_register_acceleration_x 0x28 // OUT_X_L_A
#define lsm303d_register_acceleration_y 0x2a // OUT_Y_L_A
#define lsm303d_register_acceleration_z 0x2c // OUT_Z_L_A

#define lsm303d_ctrl1_value 0x27 // 1=6.25 Hz, enable x, y and z

uint16_t lsm303dReadDoubleRegister(uint8_t address);
