#!/bin/bash

set -e

PARAM=$1
WORKDIR=$PWD

conan_package_interface()
{
    if [ `conan editable list | grep "^uart-api/"` ]
    then
        conan editable remove $(conan editable list | grep "^uart-api/")
        rm -fr interface/build/
    fi

    if [ `conan editable list | grep "^uart-api-test/"` ]
    then
        conan editable remove $(conan editable list | grep "^uart-api-test/")
        rm -fr interface/test_package/build/
    fi

    conan remove uart-api -f

    echo "build uart-api in cache"
    pushd interface
    conan create . lars/test --build=missing
    popd
}

conan_package_arduino()
{
    set -e

    if [ `conan editable list | grep "^uart-avr/"` ]
    then
        conan editable remove $(conan editable list | grep "^uart-avr/")
        rm -fr arduino/build/
    fi

    conan remove uart-avr -f

    echo "build uart-avr in cache"
    conan create arduino lars/test -pr tools/conan/profiles/gcc-avr -o uart-avr:RX_SLAVE=False
    conan create arduino lars/test -pr tools/conan/profiles/gcc-avr -o uart-avr:RX_SLAVE=True
}

conan_package_linux()
{
    set -e

    if [ `conan editable list | grep "^uart-tool/"` ]
    then
        conan editable remove $(conan editable list | grep "^uart-tool/")
        rm -fr linux/build/ 
    fi

    conan remove uart-tool -f

    echo "build uart-tool in cache"
    pushd linux
    conan create . lars/test -o uart-tool:mqtt=True -o uart-avr:RX_SLAVE=False --build=missing
    conan create . lars/test -o uart-tool:mqtt=True -o uart-avr:RX_SLAVE=True --build=missing
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
