#!/bin/bash

SCRIPT_PATH="${PWD}/tools/source_env.sh"

if [ "$0" != "-bash" ]
then
    echo "usage: source ${SCRIPT_PATH}"
else
    tools/helper_scripts/export_environment.sh

    if [ "$1" == "docker" ]
    then
        alias "${RADINO_CUSTOM_COMMAND}"="${RADINO_TOOLS_DIR}/docker/build_in_docker.sh"
        echo "${RADINO_CUSTOM_COMMAND} command ready"
    elif [ "$1" == "local" ]
    then
        alias "${RADINO_CUSTOM_COMMAND}"="${RADINO_TOOLS_DIR}/suite.sh"
        echo "${RADINO_CUSTOM_COMMAND} command ready"
    else
        echo "you must specity 'docker' or 'local' tool setup"
    fi
fi

