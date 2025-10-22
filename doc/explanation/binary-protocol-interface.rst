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
