#!/bin/bash

./tools/helper_scripts/build.sh linux
./bin/uart-tool -VD
./bin/rf-tool -k 0 -m 0 -zVD
