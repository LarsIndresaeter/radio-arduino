#!/bin/bash

set -e

PARAM=$1
WORKDIR=$PWD

conan_package_interface()
{
    if [ `conan editable list | grep "^raduino-api/"` ]
    then
        conan editable remove $(conan editable list | grep "^raduino-api/" | head -n 1)
        rm -fr interface/build/
    fi

    if [ `conan editable list | grep "^raduino-api-test/"` ]
    then
        conan editable remove $(conan editable list | grep "^raduino-api-test/" | head -n 1)
        rm -fr interface/test_package/build/
    fi

    conan remove raduino-api -f

    echo "build raduino-api in cache"
    pushd interface
    conan create . raduino/test --build=missing
    popd
}

conan_package_arduino()
{
    set -e

    if [ `conan editable list | grep "^raduino-avr/"` ]
    then
        conan editable remove $(conan editable list | grep "^raduino-avr/" | head -n 1)
        rm -fr arduino/build/
    fi

    conan remove raduino-avr -f

    echo "build raduino-avr in cache"
    conan create arduino raduino/test -pr tools/conan/profiles/gcc-avr
}

conan_package_linux()
{
    set -e

    if [ `conan editable list | grep "^raduino-gateway/"` ]
    then
        conan editable remove $(conan editable list | grep "^raduino-gateway/" | head -n 1)
        rm -fr linux/build/ 
    fi

    conan remove raduino-gateway -f

    echo "build raduino-gateway in cache"
    pushd linux
    conan create . raduino/test -o raduino-gateway:mqtt=True --build=missing
    popd
}

conan_package_all()
{
    conan_package_interface
    conan_package_arduino
    conan_package_linux
}

print_help()
{
    echo "conan_package"
    echo "    interface : create interface package"
    echo "      arduino : create arduino package"
    echo "        linux : create linux package"
    echo "          all : create all packages"
    echo "         help : print this menu"
}

if [ "${PARAM}" == "all" ]
then
    conan_package_all
elif [ "${PARAM}" == "interface" ]
then
    conan_package_interface
elif [ "${PARAM}" == "arduino" ]
then
    conan_package_arduino
elif [ "${PARAM}" == "linux" ]
then
    conan_package_linux
elif [ "${PARAM}" == "help" ]
then
    print_help
else
    echo "paramter: ${PARAM} not recognized"
    print_help
fi
