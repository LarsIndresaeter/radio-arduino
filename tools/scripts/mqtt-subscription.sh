#!/usr/bin/env bash

sendGetStatus()
{
    `sleep 1; mqttui publish raduino-subscription/RCMD '{"command":"getSubscriptionStatus"}'` &
}

getSubscriptionStatus()
{
    sendGetStatus

    SUBSCRIPTION_STATUS=$(timeout 3 mqttui read-one raduino-subscription/status 2>/dev/null)

    if [ "${SUBSCRIPTION_STATUS}" == "" ]
    then
        echo "no subscription status returned"
        exit 1
    else
        NODE_LIST_SIZE=`echo "${SUBSCRIPTION_STATUS}" | jq '.["nodes"]' | jq 'length'`

        printf "| %10s | %16s | %5s | %6s | %18s | %24s |\n" id name quad vcc accelerometer version
        printf "| %10s | %16s | %5s | %6s | %18s | %24s |\n" ---------- ---------------- ----- ------ ------------------ ------------------------
        for ((i=0; i<NODE_LIST_SIZE; i++)); do
            #echo "i:$i"
            NODE_ADDRESS=`echo "${SUBSCRIPTION_STATUS}" | jq ".[\"nodes\"][$i]"`
            NODE_NAME=`echo "${SUBSCRIPTION_STATUS}" | jq ".[\"response\"][\"${NODE_ADDRESS}\"][\"get_device_name\"][\"payload\"][\"nameString\"]"`
            VCC=`echo "${SUBSCRIPTION_STATUS}" | jq ".[\"response\"][\"${NODE_ADDRESS}\"][\"vcc\"][\"payload\"][\"vcc\"]"`
            VERSION=`echo "${SUBSCRIPTION_STATUS}" | jq ".[\"response\"][\"${NODE_ADDRESS}\"][\"get_version\"][\"payload\"][\"versionString\"]"`
            ACCELEROMETER_X=`echo "${SUBSCRIPTION_STATUS}" | jq ".[\"response\"][\"${NODE_ADDRESS}\"][\"get_lsm303d\"][\"payload\"][\"accelerometerX\"]"`
            ACCELEROMETER_Y=`echo "${SUBSCRIPTION_STATUS}" | jq ".[\"response\"][\"${NODE_ADDRESS}\"][\"get_lsm303d\"][\"payload\"][\"accelerometerY\"]"`
            ACCELEROMETER_Z=`echo "${SUBSCRIPTION_STATUS}" | jq ".[\"response\"][\"${NODE_ADDRESS}\"][\"get_lsm303d\"][\"payload\"][\"accelerometerZ\"]"`
            ACCELEROMETER=`echo "${ACCELEROMETER_X}, ${ACCELEROMETER_Y}, ${ACCELEROMETER_Z}"`
            QUAD_POS=`echo "${SUBSCRIPTION_STATUS}" | jq ".[\"response\"][\"${NODE_ADDRESS}\"][\"quadrature_encoder\"][\"payload\"][\"countpositive\"]"`
            QUAD_NEG=`echo "${SUBSCRIPTION_STATUS}" | jq ".[\"response\"][\"${NODE_ADDRESS}\"][\"quadrature_encoder\"][\"payload\"][\"countnegative\"]"`
            QUAD=`echo $QUAD_POS - $QUAD_NEG | bc`
            #echo "$NODE_ADDRESS, $NODE_NAME"
            printf "| %10s | %16s | %5s | %6s | %18s | %24s |\n" $NODE_ADDRESS $NODE_NAME $QUAD $VCC "$ACCELEROMETER" $VERSION
        done
    fi
}

listStoppedSubscriptions()
{
    sendGetStatus

    SUBSCRIPTION_STATUS=$(timeout 3 mqttui read-one raduino-subscription/status 2>/dev/null)

    if [ "${SUBSCRIPTION_STATUS}" == "" ]
    then
        echo "no subscription status returned"
        exit 1
    else
        NODE_LIST_SIZE=`echo "${SUBSCRIPTION_STATUS}" | jq '.["nodes"]' | jq 'length'`

        for ((i=0; i<NODE_LIST_SIZE; i++)); do
            #echo "i:$i"
            NODE_ADDRESS=`echo "${SUBSCRIPTION_STATUS}" | jq ".[\"nodes\"][$i]"`
            NODE_NAME=`echo "${SUBSCRIPTION_STATUS}" | jq ".[\"response\"][\"${NODE_ADDRESS}\"][\"get_device_name\"][\"payload\"][\"nameString\"]"`
            VCC=`echo "${SUBSCRIPTION_STATUS}" | jq ".[\"response\"][\"${NODE_ADDRESS}\"][\"vcc\"][\"payload\"][\"vcc\"]"`
            VERSION=`echo "${SUBSCRIPTION_STATUS}" | jq ".[\"response\"][\"${NODE_ADDRESS}\"][\"get_version\"][\"payload\"][\"versionString\"]"`
            ACCELEROMETER_X=`echo "${SUBSCRIPTION_STATUS}" | jq ".[\"response\"][\"${NODE_ADDRESS}\"][\"get_lsm303d\"][\"payload\"][\"accelerometerX\"]"`
            ACCELEROMETER_Y=`echo "${SUBSCRIPTION_STATUS}" | jq ".[\"response\"][\"${NODE_ADDRESS}\"][\"get_lsm303d\"][\"payload\"][\"accelerometerY\"]"`
            ACCELEROMETER_Z=`echo "${SUBSCRIPTION_STATUS}" | jq ".[\"response\"][\"${NODE_ADDRESS}\"][\"get_lsm303d\"][\"payload\"][\"accelerometerZ\"]"`
            ACCELEROMETER=`echo "${ACCELEROMETER_X}, ${ACCELEROMETER_Y}, ${ACCELEROMETER_Z}"`
            QUAD_POS=`echo "${SUBSCRIPTION_STATUS}" | jq ".[\"response\"][\"${NODE_ADDRESS}\"][\"quadrature_encoder\"][\"payload\"][\"countpositive\"]"`
            QUAD_NEG=`echo "${SUBSCRIPTION_STATUS}" | jq ".[\"response\"][\"${NODE_ADDRESS}\"][\"quadrature_encoder\"][\"payload\"][\"countnegative\"]"`
            QUAD=`echo $QUAD_POS - $QUAD_NEG | bc`
            #echo "$NODE_ADDRESS, $NODE_NAME"
            printf "| %10s | %16s | %5s | %6s | %18s | %24s |\n" $NODE_ADDRESS $NODE_NAME $QUAD $VCC "$ACCELEROMETER" $VERSION
        done
    fi

}

getSubscriptionStatusForNode()
{
    NODE_ADDRESS="$1"
}

if [ "$1" == "" ]
then
    echo "parameters:"
    echo "  list"
    echo "  stopped"
    echo "  enable <nodeAddress> <commandName>"
    echo "  interval <nodeAddress> <commandName> <interal>"
elif [ "$1" == "list" ]
then
    NODE_ADDRESS="${2}"

    if [ "$NODE_ADDRESS" == "" ]
    then
        getSubscriptionStatus
    else
        getSubscriptionStatusForNode "$NODE_ADDRESS"
    fi
elif [ "$1" == "stopped" ]
then
    listStoppedSubscriptions
elif [ "$1" == "enable" ]
then
    NODE_ADDRESS="${2}"
    SUBSCRIPTION_NAME="${3}"
    
    if [ "$SUBSCRIPTION_NAME" == "" ]
    then
        echo "you must supply nodeAddress and subscriptionName"
    else
        mqttui publish raduino-subscription/RCMD "{\"command\":\"enableSubscription\", \"nodeAddress\":$NODE_ADDRESS, \"name\":\"$SUBSCRIPTION_NAME\"}"
    fi
elif [ "$1" == "interval" ]
then
    NODE_ADDRESS=$2
    SUBSCRIPTION_NAME=$3
    INTERVAL=$4
    
    if [ "$INTERVAL" == "" ]
    then
        echo "you must supply nodeAddress, subscriptionName and interval"
    else
        mqttui publish raduino-subscription/RCMD "{\"command\":\"setInterval\", \"nodeAddress\":$NODE_ADDRESS, \"name\":\"$SUBSCRIPTION_NAME\", \"interval\": $INTERVAL}"
    fi
elif [ "$1" == "text" ]
then
    DISPLAY_TEXT=$2
    mqttui publish raduino-adapter/RCMD/router/1775080601 "{\"commandList\":[\"ssd1306\"], \"nodeAddress\":1775080601, \"displayText\": \"${DISPLAY_TEXT}\"}"
fi

