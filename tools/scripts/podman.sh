#!/usr/bin/env bash

echo "podman"

if [ "$1" == "local" ]
then
    podman image exists docker-daemon:radio-arduino:${TAG} && podman pull docker-daemon:radio-arduino:${TAG}
    podman run --network host -it --rm --device=/dev/ttyUSB0 radio-arduino:${TAG} bash
else
    podman run --network host -it --rm --device=/dev/ttyUSB0 ghcr.io/larsindresaeter/radio-arduino:latest bash
fi

