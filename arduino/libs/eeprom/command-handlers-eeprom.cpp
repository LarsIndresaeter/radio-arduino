#include <cmd/payloads.hxx>
#include <command-handlers-eeprom.hpp>
#include <eeprom.hpp>

namespace EEPROM {

void commandSetDeviceName(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::SET_DEVICE_NAME::command_t command(commandPayload);
    COMMANDS::SET_DEVICE_NAME::response_t response;

    EEPROM_DATA_STORE::writeToSpareAndSetAsActive(offsetof(eeprom_data_t, deviceName), &command.name[0], 16);

    response.serialize(responsePayload);
}

void commandGetDeviceName(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::GET_DEVICE_NAME::command_t command(commandPayload);
    COMMANDS::GET_DEVICE_NAME::response_t response;

    EEPROM_DATA_STORE::readFromActive(offsetof(eeprom_data_t, deviceName), &response.nameString[0], 16);

    response.serialize(responsePayload);
}

void commandEepromRead(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::EEPROM_READ::command_t command(commandPayload);
    COMMANDS::EEPROM_READ::response_t response;

    response.setAddress(command.getAddress());
    response.setData(EEPROM::read(command.getAddress()));

    response.serialize(responsePayload);
}

void commandEepromWrite(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::EEPROM_WRITE::command_t command(commandPayload);
    COMMANDS::EEPROM_WRITE::response_t response;

    EEPROM::write(command.getAddress(), command.data);

    response.setAddress(command.getAddress());
    response.setData(EEPROM::read(command.getAddress()));

    response.serialize(responsePayload);
}

void commandSetRadioRole(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::SET_RADIO_ROLE::command_t command(commandPayload);
    COMMANDS::SET_RADIO_ROLE::response_t response;

    EEPROM_DATA_STORE::setIsRadioNode(command.isRadioNode);

    response.serialize(responsePayload);
}

} // namespace EEPROM
