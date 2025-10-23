. flash arduino

flash using provided tools
==========================

There is a tools folder in the root of this repository. Use that to flash arduino

add your user to the dialout group
----------------------------------

On linux the user need to be in the dialout group to use the serial port

.. code-block:: sh

   sudo usermod -a -G dialout $USER

add udev rules
--------------

create the file `/etc/udev/rules.d/99-FTDI.rules`

.. code-block:: sh

   ACTION=="add", SUBSYSTEM=="usb", ATTRS{idVendor}=="0403", ATTRS={idProduct}=="6001", OWNER="user", MODE="0777", GROUP="dialout"

reload udev rules

.. code-block:: sh

   sudo udevadm control --reload

setup tools
-----------

before you can use the provided `raduino` tool you must source the environment tool. This has to be done for each terminal window where you want to use the `raduino` tool.

.. code-block:: sh

   source tools/setup.sh local

The command for flashing is `raduino flash <type> <device>`. Use the provided help menu to learn more. For example an arduino nano as a `gateway` can be programmed using this command.

.. code-block:: sh

   raduino flash nano gateway

flash using docker
------------------

if you are using docker to build and flash then you might need to change access rights to the serial device

.. code-block:: sh

   sudo chmod 666 /dev/ttyUSB0


