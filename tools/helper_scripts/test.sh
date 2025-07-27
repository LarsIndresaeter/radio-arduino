#!/bin/bash

_SERIAL_DEVICE=/dev/ttyUSB0
PARAMS="${PARAMS} -K secret -B -E -S -H -A -P 2 -R -D -G -C"

./bin/raduino-gateway ${PARAMS}

