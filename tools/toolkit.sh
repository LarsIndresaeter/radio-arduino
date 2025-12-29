#!/bin/bash

PARAM=$1

if [ "${PARAM}" == "info" ]
then
    echo "RADINO_REPO_BASE_DIR =${RADINO_REPO_BASE_DIR}"
    echo "RADINO_TOOLS_DIR=${RADINO_TOOLS_DIR}"
    echo "RADUINO_SCRIPTS_DIR=${RADUINO_SCRIPTS_DIR}"
    echo "RADINO_BIN_DIR=${RADINO_BIN_DIR}"
    echo "RADINO_CUSTOM_COMMAND=${RADINO_CUSTOM_COMMAND}"
fi

if [ "${PARAM}" == "podman-app" ]
then
    ./tools/scripts/podman-app.sh $2 $3 $4 $5 $6 $7
fi

if [ "${PARAM}" == "analyze" ]
then
    ${RADUINO_SCRIPTS_DIR}/analyze-avr.sh "${2}" "${3}" "${4}" ${5}" "${6}" "${7}" ${8}" "${9}"
fi

if [ "${PARAM}" == "generate" ]
then
    ${RADUINO_SCRIPTS_DIR}/generate.sh "${2}" "${3}" "${4}" ${5}" "${6}" "${7}" ${8}" "${9}"
fi

if [ "${PARAM}" == "docker-app" ]
then
    ${RADUINO_SCRIPTS_DIR}/docker-app.sh "${2}" "${3}" "${4}" ${5}" "${6}" "${7}" ${8}" "${9}"
fi

if [ "${PARAM}" == "docker-devbox" ]
then
    ${RADINO_TOOLS_DIR}/scripts/docker-devbox.sh "${2}" "${3}" "${4}" ${5}" "${6}" "${7}" ${8}" "${9}"
fi

if [ "${PARAM}" == "lint" ]
then
    ${RADUINO_SCRIPTS_DIR}/lint.sh "${2}" "${3}" "${4}" ${5}" "${6}" "${7}" ${8}" "${9}"
fi

if [ "${PARAM}" == "format" ]
then
    ${RADUINO_SCRIPTS_DIR}/format.sh "${2}" "${3}" "${4}" ${5}" "${6}" "${7}" ${8}" "${9}"
fi

if [ "${PARAM}" == "release" ]
then
    ${RADUINO_SCRIPTS_DIR}/release.sh "${2}" "${3}" "${4}" ${5}" "${6}" "${7}" ${8}" "${9}"
fi

if [ "${PARAM}" == "tool" ]
then
    ${RADUINO_SCRIPTS_DIR}/tool.sh "${2}" "${3}" "${4}" ${5}" "${6}" "${7}" ${8}" "${9}"
fi

if [ "${PARAM}" == "mqtt" ]
then
    ${RADUINO_SCRIPTS_DIR}/mqtt.sh $2 "${3}"
fi

if [ "${PARAM}" == "setup" ]
then
    ${RADUINO_SCRIPTS_DIR}/setup.sh $2
fi

if [ "${PARAM}" == "flash" ]
then
    ${RADUINO_SCRIPTS_DIR}/flash.sh $2 $3
fi

if [ "${PARAM}" == "build" ]
then
    ${RADUINO_SCRIPTS_DIR}/build.sh $2
fi

if [ "${PARAM}" == "clean" ]
then
    ${RADUINO_SCRIPTS_DIR}/clean.sh $2
fi

if [ "${PARAM}" == "help" ]
then
    ${RADUINO_SCRIPTS_DIR}/help.sh
fi

if [ "${PARAM}" == "conan" ]
then
    ${RADUINO_SCRIPTS_DIR}/create_conan_packages.sh $2
fi

if [ "${PARAM}" == "all" ]
then
    ${RADUINO_SCRIPTS_DIR}/all_clean_build.sh
fi

if [ "${PARAM}" == "experiment" ]
then
    ${RADUINO_SCRIPTS_DIR}/experiment.sh
fi

if [ "${PARAM}" == "" ]
then
    echo "invalid parameter"
    echo ""
    ${RADUINO_SCRIPTS_DIR}/help.sh
fi

