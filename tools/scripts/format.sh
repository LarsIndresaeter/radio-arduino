#!/bin/bash

PARAM=$1

if [ "${PARAM}" == "changed" ]
then
    echo "format modified c++ files using clang"
    git clang-format

elif [ "${PARAM}" == "all" ]
then
    echo "format all c++ files using clang"
    #find . -type f -regex ".*\.\(cpp\|hpp\)" -exec clang-format -i {} \;
    find . -type f -regex ".*\.\(cpp\|hpp\)" -exec clang-format -i {} \;
else
    echo "missing parameter <changed|all>"
fi

