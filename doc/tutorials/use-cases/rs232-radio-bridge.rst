rs232 radio bridge
==================

By configuring two rf-nano boards as a gateway and sending RaduinoCommanRadioUart commands you can configure the two boards to create a serial port bridge between two computers.

configure the two gateways
--------------------------

computer 1 - sender

.. code-block:: should

   ./bin/raduino-gateway -U s

computer 2 - receiver

.. code-block:: should

   ./bin/raduino-gateway -U r

open another serial program and connect to the serial port for the gateway. 

.. code-block:: should

   minicom -d /dev/ttyUSB0

