#!/bin/bash

echo "gateway on ttyUSB0 ========================"
./bin/raduino-gateway --device /dev/ttyUSB0 -xz
./bin/raduino-node --device /dev/ttyUSB0 -n 0 -Nz

echo "switch roles =============================="
./bin/raduino-node --device /dev/ttyUSB0 -n 0 -g
if [ "$(./bin/raduino-node --device /dev/ttyUSB1 -n 0 -p | grep PING)" == "complete : PING" ]
then
    exit "failed to switch role of radio node"
    exit 1
else
    #echo "node successfully switched mode to gateway. Switch other gateway to node"
    ./bin/raduino-gateway --device /dev/ttyUSB0 -n
fi

echo "gateway on ttyUSB1 ========================" ./bin/raduino-gateway --device /dev/ttyUSB1 -xz
./bin/raduino-gateway --device /dev/ttyUSB0 -n 0 -xz
./bin/raduino-node --device /dev/ttyUSB0 -n 0 -Nz

echo "switch roles =============================="
./bin/raduino-node --device /dev/ttyUSB1 -n 0 -g
if [ "$(./bin/raduino-node --device /dev/ttyUSB0 -n 0 -p | grep PING)" == "complete : PING" ]
then
    exit "failed to switch role of radio node"
    exit 1
else
    #echo "node successfully switched mode to gateway. Switch other gateway to node"
    ./bin/raduino-gateway --device /dev/ttyUSB1 -n
fi

