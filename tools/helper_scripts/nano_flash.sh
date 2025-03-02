#!/bin/bash

_SERIAL_DEVICE_MASTER=/dev/ttyUSB0
_SERIAL_DEVICE_SLAVE=/dev/ttyUSB1

PARAM=$1

flash_master()
{
    avrdude -c arduino -b 57600 -P ${_SERIAL_DEVICE_MASTER} -p atmega328p -vv -U flash:w:bin/arduino.hex
}

flash_slave()
{
    avrdude -c arduino -b 57600 -P ${_SERIAL_DEVICE_SLAVE} -p atmega328p -vv -U flash:w:bin/slave.hex
}

if [ "${PARAM}" == "slave" ] 
then
    flash_slave
elif [ "${PARAM}" == "both" ] 
then
    flash_slave
    flash_master
else 
    flash_master
fi
