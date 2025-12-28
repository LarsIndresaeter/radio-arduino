#!/bin/bash

read_signature()
{
    _DEVICE_FILE=$1
    _BAUD_RATE=$2

    avrdude -b "${_BAUD_RATE}" -p m328p -c arduino -P "${_DEVICE_FILE}"
}

get_bootload_baud_rate()
{
    _DEVICE_FILE=$1
    _BAUD_RATE=0

    # no device detected
    eval "BAUD_RATE"=$_BAUD_RATE

    _BAUD_RATE=115200
    read_signature "${_DEVICE_FILE}" "${_BAUD_RATE}"

    if [ "$?" == "0" ]
    then
        eval "BAUD_RATE"=$_BAUD_RATE
    else
        _BAUD_RATE=57600
        read_signature "${_DEVICE_FILE}" "${_BAUD_RATE}"

        if [ "$?" == "0" ]
        then
            eval "BAUD_RATE"=$_BAUD_RATE
        fi
    fi
}

check_version_and_upgrade()
{
    _DEVICE_FILE="/dev/ttyUSB0"

    _DEVICE_PING=$(./bin/raduino-system-commands -ppp)
    _DEVICE_PING=$(./bin/raduino-system-commands -ppp)
    _DEVICE_NAME=$(./bin/raduino-system-commands -m | cut -d '=' -f2 | cut -d '"' -f 2)
    echo "detected raduino gateway '${_DEVICE_NAME}'"

    if [ `./bin/raduino-system-commands -a | grep 'FAIL'` ]
    then
        echo "old version detected. start upgrade"

        get_bootload_baud_rate "${_DEVICE_FILE}"
        _BAUD_RATE=$BAUD_RATE

        if [ "${_BAUD_RATE}" == "0" ]
        then
            echo "not able to detect baud rate of arduino bootloader"
        else
            echo "baud rate detected: ${_BAUD_RATE}"
            avrdude -c arduino -b "${_BAUD_RATE}" -P "${_DEVICE_FILE}" -p atmega328p -vv -U flash:w:bin/arduino.hex

            # ping device 
            _DEVICE_PING=$(./bin/raduino-system-commands -ppp)
            _DEVICE_PING=$(./bin/raduino-system-commands -ppp)
            _DEVICE_PING=$(./bin/raduino-system-commands -ppp)
        fi
    fi

    _DEVICE_VERSION=$(./bin/raduino-system-commands -v | cut -d '=' -f2 | cut -d '"' -f 2)
    echo "gatway version '${_DEVICE_VERSION}'"
}

check_version_and_upgrade "/dev/ttyUSB0" 115200

./bin/raduino-mqtt-client

