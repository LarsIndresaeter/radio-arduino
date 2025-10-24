#!/bin/bash

# this script is copied to the dockerapp image and is intended to run inside the container
# paths are therefore local to the image and not the filesysem in the git repository
 
PARAM=$1

if [ "${PARAM}" == "rf-nano" ] 
then
    avrdude -c arduino -b 115200 -P /dev/ttyUSB0 -p atmega328p -vv -U flash:w:/bin/arduino.hex
elif [ "${PARAM}" == "nano-pro" ] 
then
    avrdude -c arduino -b 57600 -P /dev/ttyUSB0 -p atmega328p -vv -U flash:w:/bin/arduino.hex
else
    echo "expected a parameter: rf-nano or nano-pro"
fi

