# node discovery and adressing

## limitations with multiple nodes in version 1

The first version of the addressing scheme had serious limitations with respect
to communicating with multiple nodes.

- Each node had to receive a command changing the address for a gateway to talk
to multiple nodes to.
- Resetting a node requires reprogramming of the node.
- The discovery message sent from the node acts more like an advertisement than discovery.

Fixing this requires breaking changes and the major version number will increase
after fixing it.

## requirements for discovery and adressing scheme version 2

- Gateway should discover new nodes automatically
- Support for communication to multiple nodes
- Adressing nodes by unique id
- No state in the node, i.e. same behavior after node reset
- Multiple gateways can communicate with the same node

### the how

- Add the unique id programmed to eeprom to the discovery message.
- Add a command that return last id seen in a discovery message.
  This primitive allows for new nodes to be discovered.
- Add unique id to wakeup command.
  This ensures that the gateway communicate with the correct node.
- Send discovery to a broadcast channel.
- send wakeup to a channel address calcutlated from the unique id.
- Communication from wakup to next sleep period using adress calculated from
  node id.

