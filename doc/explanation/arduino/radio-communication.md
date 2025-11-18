# radio communication

In order to save power on the radio node it is important to let the cpu be in
sleep mode as much as possible. This has lead to the following design decisions.

- After a set time of no communication with the radio node it goes to sleep.
During this time the atmega328 draws 1mA of current unless you use a low power
RTC to wake it up.
- Every 5 seconds the node wakes up and sends a `discover` message. It waits
  10ms for a `wakeup` command before it sleeps another 5 seconds. This increase
  the average power consumption to 1.1mA for a board that draws no power for
  other things than the atmega328
- The node will always have the address `0` when it is powered on. After
  successfully wakeing up the node this can be changed to a value in the range `0-255`
- The [wakeup](../interface/wakeup.md) command sent to the gateway will have a
  parameter for which node to wake up.
- The address value will result in a radio channel change for the communication
  path between gateway and node. This limits communication to one node at a
  time. This is simple and easy to make but limits bandwidth when you have many nodes
