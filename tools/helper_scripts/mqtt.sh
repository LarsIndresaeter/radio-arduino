#!/bin/bash

PARAM=$1

print_help()
{
    echo "commands:"
    echo "    interval <n> : set polling interval to <n> seconds"
    echo "     text <text> : write <text> on lcd display"
    echo "      subscribe  : subscribe to radio-arduino topic"
}

if [ "${PARAM}" == "" ]
then
    print_help
elif [ "${PARAM}" == "interval" ]
then
    mosquitto_pub -t "radio-arduino/RCMD/solar-lamp" -m "{\"command\": \"desiredState\", \"pollInterval\": $2}"
    mosquitto_pub -t "radio-arduino/RCMD/breadboard" -m "{\"command\": \"desiredState\", \"pollInterval\": $2}"
elif [ "${PARAM}" == "text" ]
then
    mosquitto_pub -t "radio-arduino/RCMD/lcd" -m "{\"command\": \"lcd\", \"displayText\": \"$2\"}"
else
    echo "invalid parameter: ${PARAM}"
    print_help
fi

if [ "${PARAM}" == "subscribe" ]
then
    mosquitto_sub -t 'radio-arduino/#' -v
fi
