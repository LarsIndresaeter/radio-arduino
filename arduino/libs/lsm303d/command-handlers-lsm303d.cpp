#include <cmd/payloads.hxx>
#include <command-handlers-lsm303d.hpp>
#include <lsm303d.hpp>

namespace LSM303D {

void commandGetLsm303d(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::GET_LSM303D::command_t command(commandPayload);
    COMMANDS::GET_LSM303D::response_t response;

    response.setAccelerometerx(lsm303dReadDoubleRegister(lsm303d_register_acceleration_x));
    response.setAccelerometery(lsm303dReadDoubleRegister(lsm303d_register_acceleration_y));
    response.setAccelerometerz(lsm303dReadDoubleRegister(lsm303d_register_acceleration_z));

    response.serialize(responsePayload);
}

} // namespace LSM303D
