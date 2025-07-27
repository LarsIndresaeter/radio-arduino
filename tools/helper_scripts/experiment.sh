#!/bin/bash

./tools/helper_scripts/build.sh linux
./bin/raduino-gateway -VD
./bin/raduino-node -k 0 -m 0 -zVD
