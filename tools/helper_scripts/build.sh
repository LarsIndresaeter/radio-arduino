#!/bin/bash

set -e

PARAM=$1
WORKDIR=$PWD

mkdir -p bin

make_build_dir()
{
    BUILD_DIR=$1

    if [ -f ${BUILD_DIR}/CMakeCache.txt ]
    then

        if [ -e ${BUILD_DIR}/CMakeFiles/ ]
        then
            pushd ${BUILD_DIR}
            cmake --build .
            popd
        else
            echo "'${BUILD_DIR}/CMakeFiles' does not exist"
        fi
    else
        echo "nothing to build"
        echo "'${BUILD_DIR}/CMakeCache.txt' does not exist"
    fi
}

build_interface()
{
    make_build_dir ${WORKDIR}/build/interface
    make_build_dir ${WORKDIR}/build/interface_test
    ./build/interface_test/bin/unittest
}

build_arduino()
{
    rm -f bin/*.hex
    make_build_dir ${WORKDIR}/build/arduino
    cp build/arduino/bin/*.hex bin/
    avr-size -C --mcu=atmega328p ./build/arduino/bin/arduino.elf | grep Data | cut -d ':' -f2 | sed 's/    //g'
    make_build_dir ${WORKDIR}/build/arduino_slave
    cp build/arduino_slave/bin/arduino.hex bin/slave.hex
    avr-size -C --mcu=atmega328p ./build/arduino_slave/bin/arduino.elf | grep Data | cut -d ':' -f2 | sed 's/    //g'
}

build_linux()
{
    #make_build_dir ${WORKDIR}/linux/build/
    make_build_dir ${WORKDIR}/build/linux
    cp build/linux/apps/*/bin/* bin/
}

build_all()
{
    build_interface
    build_arduino
    build_linux
}

print_help()
{
    echo "clean"
    echo "    interface : clean interface build"
    echo "      arduino : clean arduino build"
    echo "        linux : clean linux build"
    echo "          all : clean all builds"
    echo "         help : print this menu"
}

if [ "${PARAM}" == "all" ]
then
    build_all
elif [ "${PARAM}" == "interface" ]
then
    build_interface
elif [ "${PARAM}" == "arduino" ]
then
    build_arduino
elif [ "${PARAM}" == "linux" ]
then
    build_linux
elif [ "${PARAM}" == "help" ]
then
    print_help
else
    echo "paramter: ${PARAM} not recognized"
    print_help
fi
