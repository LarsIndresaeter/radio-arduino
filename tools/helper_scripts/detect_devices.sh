#!/bin/bash

read_signature()
{
    _DEVICE_FILE=$1
    _BAUD_RATE_1=$2
    _SETTINGS_DIR="bin/devices${_DEVICE_FILE}"

    avrdude -b "${_BAUD_RATE_1}" -p m328p -c arduino -P "${_DEVICE_FILE}"

    if [ $? -eq 0 ]
    then
        mkdir -p ${_SETTINGS_DIR}
        echo "${_BAUD_RATE_1}" >> "${_SETTINGS_DIR}/baudrate"
    else
        avrdude -b "${_BAUD_RATE_1}" -p m328p -c arduino -P "${_DEVICE_FILE}"
    fi
}

read_name()
{
    _DEVICE_FILE=$1
    _SETTINGS_DIR="bin/devices${_DEVICE_FILE}"

    rm -f "${_SETTINGS_DIR}/name"

    for i in 1 2 3 4 5
    do
        sleep 1
        _PING_RESPONSE=$( ./bin/raduino-gateway --device "${_DEVICE_FILE}" -p | grep PING)

        #echo "DEBUG: ping response: ${_PING_RESPONSE}"

        if [ $? -eq 0 ]
        then
            echo "got response from ${_DEVICE_FILE}"

            ./bin/raduino-gateway --device "${_DEVICE_FILE}" -z

            _DEVICE_INFO=$(./bin/raduino-gateway --device "${_DEVICE_FILE}" -z)
            _DEVICE_NAME=$(echo "${_DEVICE_INFO}" | cut -d '=' -f2 | cut -d ',' -f1 | sed 's/ //')

            #echo "DEBUG: info: ${_DEVICE_INFO}"
            #echo "DEBUG: name: ${_DEVICE_NAME}"
            echo "${_DEVICE_NAME}" >> "${_SETTINGS_DIR}/name"

            break
        fi
    done
}

check_device_file()
{
    _DEVICE_FILE=$1
    _SETTINGS_DIR="bin/devices${_DEVICE_FILE}"
    _BAUDRATE_FILE="${_SETTINGS_DIR}/baudrate"

    rm -f "${_BAUDRATE_FILE}"

    if [ -e "${_DEVICE_FILE}" ]
    then
        read_signature "${_DEVICE_FILE}" "115200" 

        if [ ! -f "${_BAUDRATE_FILE}" ]
        then
            read_signature "${_DEVICE_FILE}" "57600" 
        fi
    fi

    if [ -e "${_BAUDRATE_FILE}" ]
    then
        read_name "${_DEVICE_FILE}"
    fi
}

check_device_file "/dev/ttyUSB0"
check_device_file "/dev/ttyUSB1"
check_device_file "/dev/ttyUSB2"

if [ -e bin/devices/gateway/device ]
then
    _DEVICE_GATEWAY=$(echo bin/devices/gateway/device)
    if [ -e "${_DEVICE_GATEWAY}" ]
    then
        echo "gateway" >> ${_DEVICE_GATEWAY/role}
    fi
fi

if [ -e bin/devices/node/device ]
then
    _DEVICE_GATEWAY=$(echo bin/devices/node/device)
    if [ -e "${_DEVICE_GATEWAY}" ]
    then
        echo "node" >> ${_DEVICE_GATEWAY/role}
    fi
fi

