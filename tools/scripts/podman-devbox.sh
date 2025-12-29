#!/bin/bash

MY_USER_ID=$(id -u)
MY_GROUP_ID=$(id -g)
USER_STRING="--user ${MY_USER_ID}:${MY_GROUP_ID}"
REPO_BASE_DIR=$(git rev-parse --show-toplevel)
CONTAINER_NAME=""

REGISTRY=$1
PARAM=$2

if [ "${REGISTRY}" == "local" ]
then
    CONTAINER_NAME="local/raduino-arduino-build"

    if [ "${PARAM}" == "build" ]
    then
        echo "build container image"
        podman build -t $CONTAINER_NAME -f tools/docker/dev/Dockerfile .
        exit 0
    elif [ "${PARAM}" == "clean" ]
    then
        echo "clean caches"
        rm -fr .conan/
        rm -fr .cache/
        rm -f .bash_history
        echo "remove old image"
        podman rmi -f radio-arduino-build
        exit 0
    fi
elif [ "${REGISTRY}" == "remote" ]
then
    CONTAINER_NAME="ghcr.io/larsindresaeter/radio-arduino-devbox:1.0.0"
else
    echo "you must specify registry: <local|remote>"
    exit 1
fi

echo "use container: ${CONTAINER_NAME}"

if [ "${PARAM}" == "bash" ]
then
    podman run ${USER_STRING} -v ${REPO_BASE_DIR}/:/home/raduino/ -it $CONTAINER_NAME /usr/bin/bash --rcfile tools/docker/.bashrc
elif [ "${PARAM}" == "all" ]
then
    podman run ${USER_STRING} -v ${REPO_BASE_DIR}/:/home/raduino/ -t $CONTAINER_NAME /usr/bin/bash -c './tools/scripts/all_clean_build.sh'
else
    echo "valid parameters: <clean|build|bash|all>"
fi

