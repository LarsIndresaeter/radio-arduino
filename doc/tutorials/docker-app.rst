docker app
==========

build docker app
----------------

.. code-block:: sh

   raduino build dockerapp

run docker app
--------------

.. code-block:: sh

   raduino dockerapp <tag>

if you do not supply a tag then a list of raduino docker apps will be listed

running a docker app without the tool suite
-------------------------------------------

if you have obtained the docker image and do not want to run the raduino suite

.. code-block::sh

    docker run -it --rm --device=/dev/ttyUSB0 raduino:0.2.0 bash

run a command

.. code-block:: sh

    docker run -it --rm --device=/dev/ttyUSB0 raduino:0.2.0 bash
    lars@332c4a94182b:/$ raduino-gateway -z
    complete : GET_DEVICE_NAME        :  nameString="rauino-gateway"
    lars@332c4a94182b:/$ exit
    exit

