#!/bin/bash

_SERIAL_DEVICE_GATEWAY=/dev/ttyUSB0
_SERIAL_DEVICE_NODE=/dev/ttyUSB1

PARAM=$1

flash_gateway_nano_pro()
{
    avrdude -c arduino -b 115200 -P ${_SERIAL_DEVICE_GATEWAY} -p atmega328p -vv -U flash:w:bin/arduino.hex
}

flash_node_nano_pro()
{
    avrdude -c arduino -b 115200 -P ${_SERIAL_DEVICE_NODE} -p atmega328p -vv -U flash:w:bin/arduino.hex
}

nano-pro_flash()
{
    local _DEVICE=$1

    if [ "${PARAM}" == "node" ] 
    then
        flash_node_nano_pro
    elif [ "${PARAM}" == "gateway" ] 
    then
        flash_gateway_nano_pro
    elif [ "${PARAM}" == "both" ] 
    then
        flash_node_nano_pro
        flash_gateway_nano_pro
    else 
        echo "missing parameter: node, gateway or both"
    fi
}

if [ "${PARAM}" == "nano-pro" ]
then
    ${RADUINO_SCRIPTS_DIR}/nano-pro_flash.sh $2
fi

if [ "${PARAM}" == "rf-nano" ]
then
    ${RADUINO_SCRIPTS_DIR}/rf-nano_flash.sh $2
fi

if [ "${PARAM}" == "auto" ]
then
    ${RADUINO_SCRIPTS_DIR}/auto_flash.sh $2
fi

if [ "${PARAM}" == "" ]
then
    echo "missing device paramter: nano-pro, rf-nano og auto"
fi
