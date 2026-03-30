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
    echo "            get  : get status from mqtt-bridge"
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
elif [ "${PARAM}" == "subscribe" ]
then
    mosquitto_sub -t 'radio-arduino/#' -v
elif [ "${PARAM}" == "start" ]
then
    docker run --rm -d --name mosquitto_broker -v "./tools/mosquitto/config/:/mosquitto/config" -p 1883:1883 eclipse-mosquitto
elif [ "${PARAM}" == "stop" ]
then
    docker stop mosquitto_broker
elif [ "${PARAM}" == "get" ]
then
    ./tools/scripts/mqtt-get.sh
else
    echo "invalid parameter: ${PARAM}"
    print_help
fi

