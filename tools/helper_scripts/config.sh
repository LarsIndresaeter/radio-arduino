#!/bin/bash

BUILD_SYSTEM=$(grep build ${RADINO_REPO_BASE_DIR}/build/config.txt)

if [ "${BUILD_SYSTEM}" == "build=docker" ]
then
    echo "configured for docker"
elif [ "${BUILD_SYSTEM}" == "build=local" ]
then
    echo "configured for local"
fi

