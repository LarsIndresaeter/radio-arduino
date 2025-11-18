# protocol encryption

Commands can be sent as either unencrypted or encrypted commands. A flag in
eeprom decide if encrypted commands are required.

When commands are encrypted the protocol payload is prepended with a 16 byte
unique block that act as the initial vector. This is described in the file
[binary protocol interface](../interface/binary-protocol-interface.md)

## protected commands

commands that will only respond if it was sent in an encrypted payload.

- [set_key](../../reference-guides/commands/set_key.md)
- [require_transport_encryption](../../reference-guides/commands/require_transport_encryption.md)
- [unlock_session](../../reference-guides/commands/unlock_session.md)

These commands must be sent encrypted even when `unlock_session` has temporarily
enabled unencrypted messages or the `require_transport_encryption` command has
permanentely disabled this feature.

