#!/bin/bash

TAG=$1

if [ "${TAG}" == "" ]
then
    echo "you must supply a version tag"
    echo ""
    docker images raduino
else
    docker run -it --rm --device=/dev/ttyUSB0 raduino:${TAG} bash
fi
