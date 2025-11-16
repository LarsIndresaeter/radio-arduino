#!/bin/bash

SCRIPT_PATH="${PWD}/tools/setup.sh"

SOURCE_COMMAND=0

# source command on linux
if [ "$0" == "-bash" ]
then
    SOURCE_COMMAND=1
fi

# source command in docker
if [ "$0" == "bash" ]
then
    SOURCE_COMMAND=1
fi

if [ "$0" == "/usr/bin/bash" ]
then
    SOURCE_COMMAND=1
fi

if [ "$SOURCE_COMMAND" != 1 ]
then
    echo "usage: source ${SCRIPT_PATH}"
else
    . tools/scripts/export_environment.sh
    alias "${RADINO_CUSTOM_COMMAND}"="${RADINO_TOOLS_DIR}/toolkit.sh"
    echo "${RADINO_CUSTOM_COMMAND} command ready"
fi

