#!/usr/bin/env bash

DEVICE_TYPE=$1
DEVICE_ROLE=$2

avrdude_flash()
{
    local _BAUD_RATE=$1
    local _DEVICE_FILE=$2

    avrdude -c arduino -b ${_BAUD_RATE} -P ${_DEVICE_FILE} -p atmega328p -vv -U flash:w:bin/arduino.hex
}

flash_device()
{
    local _BAUD_RATE=$1
    local _DEVICE_ROLE=$2

    local _SERIAL_DEVICE_GATEWAY=/dev/ttyUSB0
    local _SERIAL_DEVICE_NODE=/dev/ttyUSB1

    if [ `echo "${_DEVICE_ROLE}" | grep "^/dev/tty"` ]
    then
        avrdude_flash "${_BAUD_RATE}" "${_DEVICE_ROLE}"
    elif [ "${_DEVICE_ROLE}" == "node" ] 
    then
        avrdude_flash "${_BAUD_RATE}" "${_SERIAL_DEVICE_NODE}"
    elif [ "${_DEVICE_ROLE}" == "gateway" ] 
    then
        avrdude_flash "${_BAUD_RATE}" "${_SERIAL_DEVICE_GATEWAY}"
    elif [ "${_DEVICE_ROLE}" == "both" ] 
    then
        avrdude_flash "${_BAUD_RATE}" "${_SERIAL_DEVICE_NODE}"
        avrdude_flash "${_BAUD_RATE}" "${_SERIAL_DEVICE_GATEWAY}"
    else 
        echo "missing parameter: node, gateway, both or device file e.g. /dev/ttyUSB0"
    fi
}

if [ "${DEVICE_TYPE}" == "nano-pro" ]
then
    flash_device 57600 "${DEVICE_ROLE}"
elif [ "${DEVICE_TYPE}" == "rf-nano" ]
then
    flash_device 115200 "${DEVICE_ROLE}"
else
    echo "missing device paramter: nano-pro or rf-nano"
fi

