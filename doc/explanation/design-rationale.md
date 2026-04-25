# design rationale

## link level packages

Link level packages are radio packages that are sent between gateway and node only.

Changes to link level packages result in a upgrade of the major version.

### advertisement

Radio nodes will periodically send out advertisements.
Advertisement packages are used for discovery of radio nodes by the gateway
and broadcast of data by the radio node.

- address is shared for identification of the node
- sequence number can be used for deduplication of events when multiple
  gateways receive and forward advertisements.
- flags can be used to by the radio node to signal attention
- data can be used for sending measurements

### wakup

Arduino boards configured as gateways can wakup up radio nodes from sleep. 
This is done by sending a wake up command immediately after an advertisement package

