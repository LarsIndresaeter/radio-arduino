#!/bin/bash

set -e

PARAM=$1
WORKDIR=$PWD

delete_build_dir()
{
    BUILD_DIR=$1

    if [ -f ${BUILD_DIR}/CMakeCache.txt ]
    then

        if [ -e ${BUILD_DIR}/CMakeFiles/ ]
        then
            rm -fr ${BUILD_DIR}
        fi
    fi
}

clean_interface()
{
    delete_build_dir ${WORKDIR}/build/interface
    delete_build_dir ${WORKDIR}/build/interface_test
}

clean_arduino()
{
    delete_build_dir ${WORKDIR}/build/arduino
    delete_build_dir ${WORKDIR}/build/arduino_node
    rm -f bin/*.hex
}

clean_linux()
{
    delete_build_dir ${WORKDIR}/build/linux
    rm -f bin/*tool*
}

clean_all()
{
    clean_interface
    clean_arduino
    clean_linux
}

print_help()
{
    echo "clean"
    echo "    interface : clean interface build"
    echo "      arduino : clean arduino build"
    echo "        linux : clean linux build"
    echo "          all : clean all builds"
    echo "          dir : delete directories build/ and bin/"
    echo "         help : print this menu"
}

if [ "${PARAM}" == "all" ]
then
    clean_all
elif [ "${PARAM}" == "interface" ]
then
    clean_interface
elif [ "${PARAM}" == "arduino" ]
then
    clean_arduino
elif [ "${PARAM}" == "linux" ]
then
    clean_linux
elif [ "${PARAM}" == "dir" ]
then
    rm -fr build
    rm -fr bin
elif [ "${PARAM}" == "help" ]
then
    print_help
else
    echo "paramter: ${PARAM} not recognized"
    print_help
fi
