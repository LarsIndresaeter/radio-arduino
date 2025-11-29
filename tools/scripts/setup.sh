#!/bin/bash

set -e

PARAM=$1
WORKDIR=$PWD

setup_build_dir()
{
    BUILD_DIR=$1

    echo "setup build dir:"
}

setup_interface()
{
    ./tools/conan/editable/interface.sh
    ./tools/conan/editable/interface_test.sh
    ./build/interface_test/bin/unittest
}

setup_arduino()
{
    ./tools/conan/editable/arduino.sh
}

setup_linux()
{
    RADUINO_GATEWAY_VERSION=$(conan inspect --raw version linux)
    conan editable add linux raduino-gateway/${RADUINO_GATEWAY_VERSION}@raduino/test --layout=tools/conan/layout/linux.txt
    conan install -if build/linux linux -o raduino-gateway:mqtt=True --build=missing
    conan build -bf build/linux linux
}

setup_all()
{
    for i in $(conan editable list | grep ^raduino-)
    do
        conan editable remove ${i}
    done

    setup_interface
    setup_arduino
    setup_linux
}

print_help()
{
    echo "setup"
    echo "    interface : clean interface build"
    echo "      arduino : clean arduino build"
    echo "        linux : clean linux build"
    echo "          all : clean all builds"
    echo "         help : print this menu"
}

if ! command -v conan &> /dev/null; then
    echo "conan is not installed"
    exit 1
fi

if [ "${PARAM}" == "all" ]
then
    setup_all
elif [ "${PARAM}" == "interface" ]
then
    setup_interface
elif [ "${PARAM}" == "arduino" ]
then
    setup_arduino
elif [ "${PARAM}" == "linux" ]
then
    setup_linux
elif [ "${PARAM}" == "help" ]
then
    print_help
else
    echo "paramter: ${PARAM} not recognized"
    print_help
fi
