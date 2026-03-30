#!/usr/bin/env bash

resendBirthCertificate()
{
    sleep 1;
    mqttui publish radio-arduino/RCMD '{"command":"resendBirthCertificate"}'
}

readAndParseBirthCertificate()
{
    BIRTH_CERTIFICATE=$(timeout 1 mqttui read-one radio-arduino/DBIRTH/# 2>/dev/null)

    if [ "${BIRTH_CERTIFICATE}" == "" ]
    then
        #echo "no birth certificate returned"
        exit 1
    else
        NODE_ADDRESS=`echo "${BIRTH_CERTIFICATE}" | jq '.["nodeAddress"]'`
        NODE_NAME=`echo "${BIRTH_CERTIFICATE}" | jq '.["validResponses"]["get_device_name"]["payload"]["nameString"]'`
        LAST_ADVERTISEMENT=`echo "${BIRTH_CERTIFICATE}" | jq '.["lastAdvertisement"]'`
        TIMESTAMP=`date +%s%3N`
        TIME_SINCE_LAST_ADVERTISEMENT=`echo "($TIMESTAMP - $LAST_ADVERTISEMENT)/1000" | bc`

        printf "| %11s" $NODE_ADDRESS
        printf "| %20s" $NODE_NAME
        printf "| %10s s|" $TIME_SINCE_LAST_ADVERTISEMENT
        printf "\n"

    fi
}

resendBirthCertificate

printf "| %10s | %19s | %11s |\n" id name 'last seen'
echo "--------------------------------------------------"
readAndParseBirthCertificate
readAndParseBirthCertificate
readAndParseBirthCertificate
readAndParseBirthCertificate
readAndParseBirthCertificate

