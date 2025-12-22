#!/bin/bash

MY_USER_ID=$(id -u)
MY_GROUP_ID=$(id -g)
USER_STRING="--user ${MY_USER_ID}:${MY_GROUP_ID}"
REPO_BASE_DIR=$(git rev-parse --show-toplevel)
CONTAINER_NAME=ghcr.io/larsindresaeter/radio-arduino-devbox:1.0.0

PARAM=$1
if [ "${PARAM}" == "build" ]
then
    echo "build docker image"
    docker build -t $CONTAINER_NAME -f tools/docker/dev/Dockerfile .
elif [ "${PARAM}" == "clean" ]
then
    echo "clean caches"
    rm -fr .conan/
    rm -fr .cache/
    rm -f .bash_history
    echo "remove old image"
    docker rmi -f radio-arduino-devbox
elif [ "${PARAM}" == "run" ]
then
    docker run ${USER_STRING} -v ${REPO_BASE_DIR}/:/home/raduino/ -it $CONTAINER_NAME /usr/bin/bash --rcfile tools/docker/.bashrc
elif [ "${PARAM}" == "clean-build" ]
then
    docker run ${USER_STRING} -v ${REPO_BASE_DIR}/:/home/raduino/ -t $CONTAINER_NAME /usr/bin/bash -c './tools/scripts/all_clean_build.sh'
else
    echo "valid parameters: <clean|build|run|clean-build>"
fi

