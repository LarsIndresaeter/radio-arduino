#!/bin/bash

echo "list devices"
echo ""

printf "===================================================================\n"
printf "%10s | %18s | %20s | %10s\n" 'role' 'name' 'device' 'baud rate'
printf "===================================================================\n"

for d in bin/devices/dev/*/ ; do
    _DEV=$(echo "${d}" | cut -d '/' -f3-4)
    if [ -f "$d/name" ]
    then
        _DEVICE_NAME=$(cat "$d/name")
    else
        _DEVICE_NAME="?"
    fi

    if [ -f "$d/role" ]
    then
        _DEVICE_ROLE=$(cat "$d/role")
    else
        _DEVICE_ROLE="?"
    fi

    if [ -f "$d/baudrate" ]
    then
        _DEVICE_BAUDRATE=$(cat "$d/baudrate")
    else
        _DEVICE_BAUDRATE="?"
    fi

    printf "%10s | %18s | %20s | %10s\n" "${_DEVICE_ROLE}" "${_DEVICE_NAME}" "/${_DEV}" "${_DEVICE_BAUDRATE}"
done

