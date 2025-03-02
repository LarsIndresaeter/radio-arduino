#!/bin/bash

set -e

rm -fr build/
rm -fr bin/

./tools/helper_scripts/clean.sh all
./tools/helper_scripts/setup.sh all
./tools/helper_scripts/build.sh all
