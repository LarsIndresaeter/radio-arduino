configure device
================

help menu

.. code-block:: sh

    raduino-node
           -n : set device name
           -c : current transport key
           -t : new transport key
           -e : new encryption key
           -d : dump eeprom contents
           -r : set radio role <gateway|node>
           -h : print this text

Example 

.. code-block:: sh

   ./bin/raduion-personalize -n DEVICE-NAME -c '' -t 'secret key' -r gateway
