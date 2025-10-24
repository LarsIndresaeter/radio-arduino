#!/bin/bash

. tools/scripts/export_environment.sh

CONTAINER_NAME=radio-arduino-build
MY_USER_ID=$(id -u)
MY_GROUP_ID=$(id -g)
USER_STRING="--user ${MY_USER_ID}:${MY_GROUP_ID}"
DEVICE_STRING=""

if [ -e /dev/ttyUSB0 ]
then
    DEVICE_STRING="--device=/dev/ttyUSB0"
fi

if [ -e /dev/ttyUSB1 ]
then
    DEVICE_STRING="--device=/dev/ttyUSB0"
fi

# check if docker is istalled
if [ -x "$(command -v docker)" ]; then
    # check if image is build
    if [ "$(docker images | grep ${CONTAINER_NAME})" == "" ]
    then
        echo "you must build the docker image: $0 dockerbuild"
    fi

    # build image or build code
    if [ "$1" == "dockerbuild" ]
    then
        docker build -t $CONTAINER_NAME -f tools/docker/Dockerfile.build .
    elif [ "$2" == "dockerapp" ]
    then
        ./tools/scripts/build.sh dockerapp
    elif [ "$1" == "dockerapp" ]
    then
        ./tools/scripts/dockerapp.sh "$2"
    else
        docker run ${DEVICE_STRING} ${USER_STRING} -v ${REPO_BASE_DIR}/:/home/lars/ $CONTAINER_NAME bash -c "tools/scripts/docker.sh $1 $2 $3 $4"
    fi
else
    echo "you must install docker"
fi


