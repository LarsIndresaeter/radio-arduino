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
    . tools/helper_scripts/export_environment.sh

    if [ "$1" == "docker" ]
    then
        alias "${RADINO_CUSTOM_COMMAND}"="${RADINO_TOOLS_DIR}/docker/build_in_docker.sh"
        echo "${RADINO_CUSTOM_COMMAND} command ready"
        echo "build=docker" >> ${RADINO_REPO_BASE_DIR}/build/config.txt
    elif [ "$1" == "local" ]
    then
        alias "${RADINO_CUSTOM_COMMAND}"="${RADINO_TOOLS_DIR}/suite.sh"
        echo "${RADINO_CUSTOM_COMMAND} command ready"
        echo "build=local" >> ${RADINO_REPO_BASE_DIR}/build/config.txt
    else
        echo "you must specity 'docker' or 'local' tool setup"
    fi
fi

