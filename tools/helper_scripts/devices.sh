#!/bin/bash

PARAM=$1

if [ "${PARAM}" == "list" ]
then
    ${RADINO_HELPER_SCRIPTS_DIR}/list_devices.sh $2
fi

if [ "${PARAM}" == "detect" ]
then
    ${RADINO_HELPER_SCRIPTS_DIR}/detect_devices.sh $2
fi


if [ "${PARAM}" == "" ]
then
    echo "missing parameter"
    echo "   list : list devices"
    echo " detect : detect connected devices"
fi

