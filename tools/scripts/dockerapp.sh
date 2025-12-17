#!/bin/bash

COMMAND=$1
TAG=$2

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
        docker images raduino
    else
        docker run --network host -it --rm --device=/dev/ttyUSB0 radio-arduino:${TAG} bash
    fi
else
    echo "first parameter should be either build or run"
fi

