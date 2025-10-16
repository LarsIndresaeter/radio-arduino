#!/bin/bash

PARAM=$1

if [ "${PARAM}" == "list" ]
then
    ${RADUINO_SCRIPTS_DIR}/list_devices.sh $2
fi

if [ "${PARAM}" == "detect" ]
then
    ${RADUINO_SCRIPTS_DIR}/detect_devices.sh $2
fi


if [ "${PARAM}" == "gateway" ]
then
    _DEVICE_NAME=$2
    
    mkdir -p bin/devices/gateway/

    if [ "${_DEVICE_NAME}" == "" ]
    then
        if [ -f bin/devices/gateway/device ]
        then
            cat bin/devices/gateway/device
        fi
    elif [ -e "${_DEVICE_NAME}" ]
    then
        echo "${_DEVICE_NAME}" > bin/devices/gateway/device

        if [ -e "bin/devices${_DEVICE_NAME}" ]
        then
            echo "gateway" > "bin/devices${_DEVICE_NAME}/role"
        fi
    else
        echo "device does not exist"
    fi
fi

if [ "${PARAM}" == "node" ]
then
    _DEVICE_NAME=$2
    
    mkdir -p bin/devices/node/

    if [ "${_DEVICE_NAME}" == "" ]
    then
        if [ -f bin/devices/node/device ]
        then
            cat bin/devices/node/device
        fi
    elif [ -e "${_DEVICE_NAME}" ]
    then
        echo "${_DEVICE_NAME}" > bin/devices/node/device

        if [ -e "bin/devices${_DEVICE_NAME}" ]
        then
            echo "node" > "bin/devices${_DEVICE_NAME}/role"
        fi
    else
        echo "device does not exist"
    fi
fi

if [ "${PARAM}" == "" ]
then
    echo "missing parameter"
    echo "               list : list devices"
    echo "             detect : detect connected devices"
    echo "   gateway <device> : get or set a device as gateway or node"
    echo "      node <device> : get or set a device as gateway or node"
fi

