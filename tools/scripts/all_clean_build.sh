#!/bin/bash

set -e

rm -fr build/
rm -fr bin/

./tools/scripts/clean.sh all
./tools/scripts/setup.sh all
./tools/scripts/build.sh all
