#!/bin/bash

echo "list devices"

printf "==================================================================\n"
printf "%20s | %20s | %20s\n" 'role' 'name' 'device'
printf "==================================================================\n"

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

    printf "%20s | %20s | %20s\n" "${_DEVICE_ROLE}" "${_DEVICE_NAME}" "${_DEV}"
done

