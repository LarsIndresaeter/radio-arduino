#!/bin/bash

COMMAND=$1
TAG=$2

function checkTty()
{
    local TTY=/dev/ttyUSB0

    if [ -e ${TTY} ]
    then
        if [ $(ls -l ${TTY} | grep -c 'rw-rw-rw') == "0" ]
        then
            echo "give read and write access to all users for ${TTY}"
            sudo chmod a+rw ${TTY}
        else
            echo "all users have access to ${TTY}"
        fi
    else
        echo "no ${TTY} present"
    fi
}

if [ "${COMMAND}" == "build" ]
then
    TAG_LATEST=$(git describe --tags --abbrev=0)
    docker build -t radio-arduino:${TAG_LATEST} --file tools/docker/app/Dockerfile .
elif [ "${COMMAND}" == "run" ]
then
    if [ "${TAG}" == "" ]
    then
        echo "you must supply a version tag"
        echo ""
        docker images radio-arduino
    else
        checkTty
        docker run --network host -it --rm --device=/dev/ttyUSB0 radio-arduino:${TAG} bash
    fi
else
    echo "first parameter should be either build or run"
fi

