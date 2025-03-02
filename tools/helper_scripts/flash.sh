#!/bin/bash

PARAM=$1

if [ "${PARAM}" == "nano" ]
then
    ${RADINO_HELPER_SCRIPTS_DIR}/nano_flash.sh $2
fi

if [ "${PARAM}" == "rf-nano" ]
then
    ${RADINO_HELPER_SCRIPTS_DIR}/rf-nano_flash.sh $2
fi

if [ "${PARAM}" == "" ]
then
    echo "missing paramter: nano or rf-nano"
fi
