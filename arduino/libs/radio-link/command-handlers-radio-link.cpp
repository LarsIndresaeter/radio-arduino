#include <cmd/payloads.hxx>
#include <command-handlers-radio-link.hpp>
#include <radio-link.hpp>

extern bool rx_mode_gateway;

namespace RADIOLINK {

void commandWakeup(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::WAKEUP::command_t command(commandPayload);
    COMMANDS::WAKEUP::response_t response;

    if (rx_mode_gateway) {
        response.discovered = RADIOLINK::sendWakeupCommandToNode(command.checkAttentionFlag);
    }

    response.serialize(responsePayload);
}

void commandSetNodeAddress(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::SET_NODE_ADDRESS::command_t command(commandPayload);
    COMMANDS::SET_NODE_ADDRESS::response_t response;

    RADIOLINK::setNodeAddress(command.nodeAddress);

    response.serialize(responsePayload);
}

} // namespace RADIOLINK
