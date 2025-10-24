docker app
==========

install docker (ubuntu 24.04)
-----------------------------

.. code-block:: sh

   sudo apt-get install docker.io
   sudo usermod -aG docker $USER
   newgrp

log out and in again

.. code-block:: sh

   docker version
   docker run hello-world

if this fails then find an online tutorial and follow the steps until you can successfully run the `docker run hello-world` command.

give user access so serial device
---------------------------------

add user to dialout group

.. code-block:: sh

   sudo usermod -aG dialout $USER
   newgrp

create file `/etc/udev/rules.d/60-ch340.rules`

.. code-block::

    ATTRS{idVendor}=="1a86", ATTRS{idProduct}=="7523", ENV{ID_MM_DEVICE_MANUAL_SCAN_ONLY}="1"
    SUBSYSTEM=="dialout", MODE="0666", GROUP="dialout", ATTRS{idVendor}=="1a86", ATTRS{idProduct}=="7523"

reload udev rules

.. code-block:: sh

   sudo udevadm control --reload

change access to device file

.. code-block:: sh

   sudo chmod a+rw /dev/ttyUSB0

build docker app
----------------

.. code-block:: sh

   raduino dockerapp build

run docker app
--------------

.. code-block:: sh

   raduino dockerapp run <tag>

if you do not supply a tag then a list of raduino docker apps will be listed

loading an image from a tar file
--------------------------------

.. code-block:: sh

   docker image load -i raduino-0.2.0.tar

running a docker app without the tool suite
-------------------------------------------

if you have obtained the docker image and do not want to run the raduino suite

.. code-block:: sh

    docker run -it --rm --device=/dev/ttyUSB0 raduino:0.2.0 bash

run a command

.. code-block:: sh

    docker run -it --rm --device=/dev/ttyUSB0 raduino:0.2.0 bash
    lars@332c4a94182b:/$ raduino-gateway -z
    complete : GET_DEVICE_NAME        :  nameString="rauino-gateway"
    lars@332c4a94182b:/$ exit
    exit

