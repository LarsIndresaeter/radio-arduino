#include <cmd/payloads.hxx>
#include <command-handlers-i2c.hpp>
#include <i2c.hpp>

namespace COMMAND_HANDLERS{

void commandI2cWrite(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::I2C_WRITE::command_t command(commandPayload);
    COMMANDS::I2C_WRITE::response_t response;

    I2C_Init();
    I2C_Start(command.device); // write address
    I2C_Write(command.registerAddress[0]); // first word address
    I2C_Write(command.registerAddress[1]); // second word address
    for (uint8_t i = 0;
         (i < command.length) && (i < sizeof(command.data));
         i++) {
        if (0 != I2C_Write(command.data[i])) {
            break;
        }
    }
    I2C_Stop();

    response.serialize(responsePayload);
}

void commandI2cRead(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::I2C_READ::command_t command(commandPayload);
    COMMANDS::I2C_READ::response_t response;

    response.setDevice(command.getDevice());
    response.setRegisteraddress(command.getRegisteraddress());
    response.setLength(command.getLength());

    I2C_Init();
    I2C_Start(command.device); // read address
    I2C_Write(command.registerAddress[0]); // first word address
    I2C_Write(command.registerAddress[1]); // second word address

    I2C_Repeated_Start(command.device + 1);

    for (uint8_t i = 0;
         (i < command.length) && (i < sizeof(response.data));
         i++) {
        response.data[i] = I2C_Read_Ack();
    }

    I2C_Read_Nack(); /* Read flush data with nack */
    I2C_Stop();

    response.serialize(responsePayload);
}

} // namespace COMMAND_HANDLERS

