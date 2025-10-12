#!/bin/bash

_SERIAL_DEVICE_GATEWAY=/dev/ttyUSB0
_SERIAL_DEVICE_NODE=/dev/ttyUSB1
PARAM=$1

flash_gateway()
{
    avrdude -c arduino -b 115200 -P ${_SERIAL_DEVICE_GATEWAY} -p atmega328p -vv -U flash:w:bin/arduino.hex
}

flash_node()
{
    avrdude -c arduino -b 115200 -P ${_SERIAL_DEVICE_NODE} -p atmega328p -vv -U flash:w:bin/arduino.hex
}

if [ "${PARAM}" == "node" ] 
then
    flash_node
elif [ "${PARAM}" == "gateway" ] 
then
    flash_gateway
elif [ "${PARAM}" == "both" ] 
then
    flash_node
    flash_gateway
else 
    echo "missing parameter: node, gateway or both"
fi
