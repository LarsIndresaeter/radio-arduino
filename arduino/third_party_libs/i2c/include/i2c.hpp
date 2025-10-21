// https://www.electronicwings.com/avr-atmega/atmega1632-i2c

#include <avr/io.h>     /* Include AVR std. library file */
#include <util/delay.h> /* Include Delay header file */

// #define BITRATE(TWSR)	((F_CPU/SCL_CLK)-16)/(2*pow(4,(TWSR&((1<<TWPS0)|(1<<TWPS1)))))

void I2C_Init();
uint8_t I2C_Start(uint8_t address);
uint8_t I2C_Repeated_Start(uint16_t address);
uint8_t I2C_Write(uint8_t data);
char I2C_Read_Ack();
char I2C_Read_Nack();
void I2C_Stop();
