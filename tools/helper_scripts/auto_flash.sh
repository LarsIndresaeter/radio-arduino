#!/bin/bash

_SERIAL_DEVICE_GATEWAY=$(cat bin/devices/gateway/device)
_SERIAL_DEVICE_NODE=$(cat bin/devices/node/device)
_BAUDRATE_GATEWAY=$(cat bin/devices${_SERIAL_DEVICE_GATEWAY}/baudrate)
_BAUDRATE_NODE=$(cat bin/devices${_SERIAL_DEVICE_NODE}/baudrate)

PARAM=$1

flash_gateway()
{
    if [ "" == "${_SERIAL_DEVICE_GATEWAY}" ]
    then
        echo "name of device file from is empty."
        echo "run the command: raduino devices list"
    elif [ "" == "${_BAUDRATE_GATEWAY}" ]
    then
        echo "baud rate device file from is empty."
        echo "run the command: raduino devices list"
    else
        avrdude -c arduino -b ${_BAUDRATE_GATEWAY} -P ${_SERIAL_DEVICE_GATEWAY} -p atmega328p -vv -U flash:w:bin/arduino.hex
    fi
}

flash_arduino()
{
    if [ "" == "${_SERIAL_DEVICE_NODE}" ]
    then
        echo "name of device file from is empty."
        echo "run the command: raduino devices list"
    elif [ "" == "${_BAUDRATE_NODE}" ]
    then
        echo "baud rate device file from is empty."
        echo "run the command: raduino devices list"
    else
        avrdude -c arduino -b ${_BAUDRATE_NODE} -P ${_SERIAL_DEVICE_NODE} -p atmega328p -vv -U flash:w:bin/arduino.hex
    fi
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
    echo "specify device: node, gateway or both"
fi
