#!/bin/bash

. tools/helper_scripts/export_environment.sh

CONTAINER_NAME=cxx-uart-build
MY_USER_ID=$(id -u)
MY_GROUP_ID=$(id -g)
USER_STRING="--user ${MY_USER_ID}:${MY_GROUP_ID}"

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
        docker build -t $CONTAINER_NAME tools/docker/
    else
        docker run ${USER_STRING} -v ${REPO_BASE_DIR}/:/home/lars/ $CONTAINER_NAME bash -c "tools/helper_scripts/docker.sh $1 $2 $3 $4"
    fi
else
    echo "you must install docker"
fi


