# protocol encryption

Commands can be sent as either unencrypted or encrypted commands. A flag in eeprom decide if encrypted commands are required.

When commands are encrypted the protocol payload is prepended with a 16 byte uniq block that act as the initial vector. This is described in the file [binary protocol interface](../binary-protocol-interface.md)

