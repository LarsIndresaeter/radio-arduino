#!/usr/bin/env bash

if [ "$1" == "local" ]
then
    podman image exists docker-daemon:radio-arduino:${TAG} && podman pull docker-daemon:radio-arduino:${TAG}
    podman run --network host -it --rm --device=/dev/ttyUSB0 radio-arduino:${TAG} bash
elif [ "$1" == "remote" ]
then
    podman run --network host -it --rm --device=/dev/ttyUSB0 ghcr.io/larsindresaeter/radio-arduino:latest bash
else
    echo "invalid parameter: <local|remote>"
fi

