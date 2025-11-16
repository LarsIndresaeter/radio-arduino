#!/bin/bash

SCRIPT_PATH="${PWD}/tools/setup.sh"

SOURCE_COMMAND=0

if [ "$0" == "-bash" ]
then
    SOURCE_COMMAND=1
fi

if [ "$0" == "bash" ]
then
    SOURCE_COMMAND=1
fi

if [ "$SOURCE_COMMAND" != 1 ]
then
    echo "usage: source ${SCRIPT_PATH}"
else
    . tools/scripts/export_environment.sh

    if [ "$1" == "docker" ]
    then
        alias "${RADINO_CUSTOM_COMMAND}"="${RADINO_TOOLS_DIR}/scripts/run_in_docker.sh"
        echo "${RADINO_CUSTOM_COMMAND} command ready"
    elif [ "$1" == "local" ]
    then
        alias "${RADINO_CUSTOM_COMMAND}"="${RADINO_TOOLS_DIR}/toolkit.sh"
        echo "${RADINO_CUSTOM_COMMAND} command ready"
    else
        echo "you must specity 'docker' or 'local' tool setup"
    fi
fi

