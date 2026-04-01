#!/usr/bin/env bash

sendGetStatus()
{
    `sleep 1; mqttui publish raduino-subscriptions/RCMD '{"command":"getSubscriptionStatus"}'` &
}

getSubscriptionStatus()
{
    sendGetStatus

    SUBSCRIPTION_STATUS=$(timeout 3 mqttui read-one raduino-subscriptions/status 2>/dev/null)

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


getSubscriptionStatus

