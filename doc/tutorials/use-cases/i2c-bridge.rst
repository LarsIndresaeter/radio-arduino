i2c bridge
==========

generic tool. useful for test and developement. You will typically use this to experiment with a new i2c device before adding a new command specifically for that device.

example
-------

read register 0 from ina219

* ina219 address = 0x80
* default value of register 0, configuration = 0x399F

.. code-block:: sh

   ./bin/raduino-i2c-bridge -d 128 -o 5 -r 2

