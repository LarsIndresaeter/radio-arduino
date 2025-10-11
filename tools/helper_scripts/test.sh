#!/bin/bash

echo "gateway on ttyUSB0 ========================"
./bin/raduino-gateway --device /dev/ttyUSB0 -xz
./bin/raduino-node --device /dev/ttyUSB0 -n 0 -Nz

echo "switch roles =============================="
./bin/raduino-node --device /dev/ttyUSB0 -n 0 -g
./bin/raduino-gateway --device /dev/ttyUSB0 -n

echo "gateway on ttyUSB1 ========================"
./bin/raduino-gateway --device /dev/ttyUSB1 -xz
./bin/raduino-node --device /dev/ttyUSB1 -n 0 -Nz

echo "switch roles =============================="
./bin/raduino-node --device /dev/ttyUSB1 -n 0 -g
./bin/raduino-gateway --device /dev/ttyUSB1 -n

