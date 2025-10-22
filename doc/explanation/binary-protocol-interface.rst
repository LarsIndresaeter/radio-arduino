binary protocol interface
=========================
   
packet
------

+-------------------------+----------------+
| byte                    | description    |
+=========================+================+
| 0-1                     | sync = 0xFEED  |
| 2                       | version        |
| 3                       | length         |
| 4                       | packet payload |
| (4+length)-(4+length+4) | checksum       |
+-------------------------+----------------+

encrypted packet
----------------

+---------------------------+-----------------+
| byte                      | description     |
+===========================+=================+
| 0-1                       | sync = 0xFEED   |
| 2                         | version         |
| 3                         | length          |
| 4                         | inner checksum  |
| 8                         | message counter |
| 12                        | nonce           |
| 16                        | reserved        |
| 20                        | packet payload  |
| (20+length)-(20+length+4) | checksum        |
+---------------------------+-----------------+

byte 4-14 (first 16 byte, ie. 128 bit) is the IV for the CBC-CTS encryption algorithm.

packet payload (command)
------------------------

+----------+-----------------------------------------+
| byte     | description                             |
+==========+=========================================+
| 0        | Object Identifier (OI), aka. command id |
| 1        | Object Length (OL)                      |
| 2-(2+OL) | command payload                         |
+----------+-----------------------------------------+

protocol defines
----------------

The protocol defined in the file `interface/libs/protocol/include/protocolBase.hpp` is used by the arduino code and the linux code. This and other header files are part of the `raduino-interface` header only conan package.

`constexpr` is used to define constants organized in C++ namespace.

The following example of how to use these constants is taken from the file `interface/libs/protocol/include/streamProtocol.hpp`
.. code-block:: c++

    if (index == 1) {
        if (c != PROTOCOL::HEADER::SYNC_PATTERN_BYTE_1) {
            return 0;
        }
    }

cryptoHandlerInterface
----------------------

The file `interface/libs/protocol/include/cryptoHandlerInterface.hpp` define the interface for the cryptoHandlerInterface. The implementation is done in the arduino and linux source folders.

example command
===============

eeprom_read
-----------

command payload
'''''''''''''''

+------+-------------+-------|
| byte | description | value |
+======+=============+=======+
| 0    | 6           | 6     |
| 1    | 2           | 2     |
| 2-3  | address     | 2     |
+------+-------------+-------+

binary: 06 02 00 02

response payload
''''''''''''''''

+------+-------------+-------|
| byte | description | value |
+======+=============+=======+
| 0    | OI          | 6     |
| 1    | OL          | 3     |
| 2-3  | address     | 2     |
| 4    | data        | 3     |
+------+-------------+-------+

binary: 06 03 00 02 03

display binary data using the verbose flag
------------------------------------------

example using the ping command `./bin/raduino-gateway -Vp`

This is a command with no command payload and thus the size of the protocol packet is the minimum size of 10 bytes.

.. code-block:: sh

    w: FE ED 01 02 21 00 F2 BA F7 FA 
    r: FE ED 01 02 21 00 F2 BA F7 FA 
    [2566]complete : PING                   :

sync: FE ED
protocol version: 01
protocol payload length: 02
OI: 21
OL: 00
Checksum: F2 BA F7 FA
