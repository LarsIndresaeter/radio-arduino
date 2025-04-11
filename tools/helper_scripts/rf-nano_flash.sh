#!/bin/bash

_SERIAL_DEVICE_MASTER=/dev/ttyUSB0
_SERIAL_DEVICE_NODE=/dev/ttyUSB1
PARAM=$1

flash_master()
{
    avrdude -c arduino -b 115200 -P ${_SERIAL_DEVICE_MASTER} -p atmega328p -vv -U flash:w:bin/arduino.hex
}

flash_node()
{
    avrdude -c arduino -b 115200 -P ${_SERIAL_DEVICE_NODE} -p atmega328p -vv -U flash:w:bin/node.hex
}

if [ "${PARAM}" == "node" ] 
then
    flash_node
elif [ "${PARAM}" == "master" ] 
then
    flash_master
elif [ "${PARAM}" == "both" ] 
then
    flash_node
    flash_master
else 
    echo "missing parameter: node, master or both"
fi
