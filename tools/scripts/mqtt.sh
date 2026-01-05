#!/bin/bash

PARAM=$1

print_help()
{
    echo "commands:"
    echo "interval <n> <t> : set polling interval for node <n> to <s> seconds"
    echo "     text <text> : write <text> on lcd display"
    echo "      subscribe  : subscribe to radio-arduino topic"
    echo "          start  : start mosquitto broker"
    echo "           stop  : stop mosquitto broker"
}
 
if [ "${PARAM}" == "" ]
then
    print_help
elif [ "${PARAM}" == "interval" ]
then
    mosquitto_pub -t "radio-arduino/RCMD/$2" -m "{\"command\": \"desiredState\", \"pollInterval\": $3}"
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

if [ "${PARAM}" == "start" ]
then
    docker run --rm -d --name mosquitto_broker -v "./tools/mosquitto/config/:/mosquitto/config" -p 1883:1883 eclipse-mosquitto
fi

if [ "${PARAM}" == "stop" ]
then
    docker stop mosquitto_broker
fi

