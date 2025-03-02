#!/bin/bash

#./tools/all_clean_build.sh
./tools/helper_scripts/build.sh arduino
./tools/helper_scripts/rf-nano_flash.sh both
./bin/uart-tool -DD
./bin/rf-tool -HHwHH

