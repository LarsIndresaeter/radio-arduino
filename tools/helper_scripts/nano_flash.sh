#!/bin/bash

_SERIAL_DEVICE_GATEWAY=/dev/ttyUSB0
_SERIAL_DEVICE_NODE=/dev/ttyUSB1

PARAM=$1

flash_gateway()
{
    avrdude -c arduino -b 57600 -P ${_SERIAL_DEVICE_GATEWAY} -p atmega328p -vv -U flash:w:bin/arduino.hex
}

flash_node()
{
    avrdude -c arduino -b 57600 -P ${_SERIAL_DEVICE_NODE} -p atmega328p -vv -U flash:w:bin/node.hex
}

if [ "${PARAM}" == "node" ] 
then
    flash_node
elif [ "${PARAM}" == "both" ] 
then
    flash_node
    flash_gateway
else 
    flash_gateway
fi
