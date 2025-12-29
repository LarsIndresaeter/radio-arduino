#!/bin/bash

REGISTRY=$1
COMMAND=$2
TAG=$3

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

if [ "${REGISTRY}" == "local" ]
then
    _CONTAINER="localhost/radio-arduino"

    if [ "${COMMAND}" == "build" ]
    then
        TAG_LATEST=$(git describe --tags --abbrev=0)
        podman build -t ${_CONTAINER}:${TAG_LATEST} --file tools/docker/app/Dockerfile .
    elif [ "${COMMAND}" == "run" ]
    then
        if [ "${TAG}" == "" ]
        then
            echo "you must supply a version tag"
            echo ""
            podman images ${_CONTAINER}
        else
            checkTty
            podman run --network host -it --rm --device=/dev/ttyUSB0 ${_CONTAINER}:${TAG} bash
        fi
    else
        echo "first parameter should be either build or run"
    fi
elif [ "${REGISTRY}" == "remote" ]
then
    if [ "${COMMAND}" == "run" ]
    then
        if [ "${TAG}" == "" ]
        then
            echo "you must supply a version tag. latest or semver"
        else
            _CONTAINER="ghcr.io/larsindresaeter/radio-arduino"
            podman run --network host -it --rm --device=/dev/ttyUSB0 ${_CONTAINER}:${TAG} bash
        fi
    else
        echo "only valid verb for remote registry is <run>"
    fi
else
    echo "you must specify a valid registry location: <local|remote>"
fi


