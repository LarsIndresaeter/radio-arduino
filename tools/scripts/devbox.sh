#!/bin/bash

MY_USER_ID=$(id -u)
MY_GROUP_ID=$(id -g)
USER_STRING="--user ${MY_USER_ID}:${MY_GROUP_ID}"
REPO_BASE_DIR=$(git rev-parse --show-toplevel)
CONTAINER_NAME=radio-arduino-build

PARAM=$1
if [ "${PARAM}" == "build" ]
then
    echo "build docker image"
    docker build -t $CONTAINER_NAME -f tools/docker/Dockerfile.build .
elif [ "${PARAM}" == "clean" ]
then
    echo "clean caches"
    rm -fr .conan/
    rm -fr .cache/
    rm -f .bash_history
    echo "remove old image"
    docker rmi -f radio-arduino-build
elif [ "${PARAM}" == "run" ]
then
    docker run ${USER_STRING} -v ${REPO_BASE_DIR}/:/home/raduino/ -it $CONTAINER_NAME /usr/bin/bash --rcfile tools/docker/.bashrc
else
    echo "valid parameters: clean,build,run"
fi

