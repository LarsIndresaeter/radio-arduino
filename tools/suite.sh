#!/bin/bash

PARAM=$1

if [ "${PARAM}" == "info" ]
then
    echo "RADINO_REPO_BASE_DIR =${RADINO_REPO_BASE_DIR}"
    echo "RADINO_TOOLS_DIR=${RADINO_TOOLS_DIR}"
    echo "RADINO_HELPER_SCRIPTS_DIR=${RADINO_HELPER_SCRIPTS_DIR}"
    echo "RADINO_BIN_DIR=${RADINO_BIN_DIR}"
    echo "RADINO_CUSTOM_COMMAND=${RADINO_CUSTOM_COMMAND}"
fi

if [ "${PARAM}" == "devices" ]
then
    ${RADINO_HELPER_SCRIPTS_DIR}/devices.sh "${2}"
fi

if [ "${PARAM}" == "mqtt" ]
then
    ${RADINO_HELPER_SCRIPTS_DIR}/mqtt.sh $2 "${3}"
fi

if [ "${PARAM}" == "setup" ]
then
    ${RADINO_HELPER_SCRIPTS_DIR}/setup.sh $2
fi

if [ "${PARAM}" == "flash" ]
then
    ${RADINO_HELPER_SCRIPTS_DIR}/flash.sh $2 $3
fi

if [ "${PARAM}" == "build" ]
then
    ${RADINO_HELPER_SCRIPTS_DIR}/build.sh $2
fi

if [ "${PARAM}" == "clean" ]
then
    ${RADINO_HELPER_SCRIPTS_DIR}/clean.sh $2
fi

if [ "${PARAM}" == "config" ]
then
    ${RADINO_HELPER_SCRIPTS_DIR}/config.sh
fi

if [ "${PARAM}" == "experiment" ]
then
    ${RADINO_TOOLS_DIR}/experiment.sh
fi

if [ "${PARAM}" == "help" ]
then
    ${RADINO_HELPER_SCRIPTS_DIR}/help.sh
fi

if [ "${PARAM}" == "test" ]
then
    ${RADINO_HELPER_SCRIPTS_DIR}/test.sh
fi

if [ "${PARAM}" == "conan" ]
then
    ${RADINO_HELPER_SCRIPTS_DIR}/create_conan_packages.sh $2
fi

if [ "${PARAM}" == "all" ]
then
    ${RADINO_HELPER_SCRIPTS_DIR}/all_clean_build.sh
fi

if [ "${PARAM}" == "experiment" ]
then
    ${RADINO_HELPER_SCRIPTS_DIR}/experiment.sh
fi

if [ "${PARAM}" == "" ]
then
    echo "invalid parameter"
    echo ""
    ${RADINO_HELPER_SCRIPTS_DIR}/help.sh
fi
