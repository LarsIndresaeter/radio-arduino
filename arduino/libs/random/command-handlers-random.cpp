#include <cmd/payloads.hxx>
#include <command-handlers-random.hpp>
#include <random.hpp>

namespace COMMAND_HANDLERS {

void commandRandom(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::RANDOM::response_t response;

    RANDOM::addEntropyAndMix();

    for (uint8_t i = 0; i < sizeof(response.data); i++) {
        response.data[i] = RANDOM::getRandomByte();
    }

    response.serialize(responsePayload);
}

} // namespace COMMAND_HANDLERS
