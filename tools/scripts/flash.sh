#!/bin/bash

PARAM=$1

if [ "${PARAM}" == "nano-pro" ]
then
    ${RADUINO_SCRIPTS_DIR}/nano-pro_flash.sh $2
fi

if [ "${PARAM}" == "rf-nano" ]
then
    ${RADUINO_SCRIPTS_DIR}/rf-nano_flash.sh $2
fi

if [ "${PARAM}" == "auto" ]
then
    ${RADUINO_SCRIPTS_DIR}/auto_flash.sh $2
fi

if [ "${PARAM}" == "" ]
then
    echo "missing device paramter: nano-pro, rf-nano og auto"
fi
