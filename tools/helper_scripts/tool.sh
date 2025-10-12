#!/bin/bash

PARAM=$1

if [ "${PARAM}" == "personalize" ]
then
    ./bin/raduino-personalize $2 $3 $4 $5 $6 $7
fi

if [ "${PARAM}" == "gateway" ]
then
    ./bin/raduino-gateway $2 $3 $4 $5 $6 $7
fi

if [ "${PARAM}" == "node" ]
then
    ./bin/raduino-node $2 $3 $4 $5 $6 $7
fi

if [ "${PARAM}" == "test" ]
then
    ./bin/raduino-test $2 $3 $4 $5 $6 $7
fi

if [ "${PARAM}" == "mqtt" ]
then
    ./bin/raduino-mqtt $2 $3 $4 $5 $6 $7
fi

if [ "${PARAM}" == "power" ]
then
    ./bin/power-monitor $2 $3 $4 $5 $6 $7
fi

if [ "${PARAM}" == "" ]
then
    echo "missing tool paramter:"
    echo "     personalize : used for setting values in eeprom, typically a new device"
    echo "         gateway : communicate with the gateway"
    echo "            node : communicate with a node through a gateway"
    echo "            test : miscellaneous test commands"
    echo "            mqtt : read data from nodes and publish to mqtt broker"
    echo "            power: read data from gateway with ina219 power sensor and publish to mqtt broker"
fi
