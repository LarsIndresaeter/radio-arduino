#!/bin/bash

PARAM=$1
REPO_BASE_DIR=$(git rev-parse --show-toplevel)

if [ "${PARAM}" == "commands" ]
then
    cd ${REPO_BASE_DIR}/interface/libs/commands/
    python3 generate.py
else
    echo "missing parameter <commands>"
fi

