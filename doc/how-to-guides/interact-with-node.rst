raduino-node
============

the application `raduino-node` is used for communication with the node device.

.. code-block:: sh

    raduino-node
           -V : Verbose on
           -v : Verbose off
           -B : Blink command
           -e : EEPROM command
           -H : HOTP command
           -P : pwm command
           -R : get random bytes command
           -C : print counter values
           -D : debug command
           -t : disable transport encryption
           -T : enable transport encryption
           -x : get statistics
           -E : set AES Key
           -g : reboot node as gateway
           -Z : set device name
           -z : get device name
           -j : read vcc
           -s : sleep
           -w : wake up sleeping rx node
           -N : wake up sleeping rx node if data available flag is set
           -q : read quadrature encoder
           -A : read quadrature encoder on change
           -n : wakeup node address
           -a : update node address
           -k : set keep alive interval
           -p : ping radio node
           -K : set transport key on device (command must be encrypted)
           -b : use transport key
           -r : set transport encryption required (command must be encrypted)
           -u : unencrypted session (command must be encrypted)
           -h : print this text

a typical first command is to wake up node with address 0, ping and request the node name

.. code-block:: sh

   ./bin/raduino-node -n 0 -p -z

if you have enable encryption of the binary packages during the personalization step then you need to add some options

.. code-block:: sh

   ./bin/raduino-node -n 0 -b "secret key" -T -p -z

