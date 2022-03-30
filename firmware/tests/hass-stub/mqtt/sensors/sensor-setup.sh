#!/bin/sh

BASENAME=$(dirname "$0")

FILES=`ls $BASENAME/devices-discovery/*.json`

for device in $FILES; do
    device_id=$(basename "$device" .json)
    echo "$device => homeassistant/sensor/$device_id/config"
    mosquitto_pub -h mqtt -t "homeassistant/sensor/$device_id/config" -f $device

    TOPIC=`sed -n 's/.*\"state_topic\": \"\(.*\)\",/\1/p' $device`
    SENSOR_TYPE=${TOPIC##*/}
    echo $SENSOR_TYPE
    case $SENSOR_TYPE in
        temperature)
            VALUE=$(($RANDOM%6+19))
            ;;
        humidity)
            VALUE=$(($RANDOM%15+40))
            ;;
        airpressure)
            VALUE=$(($RANDOM%20+990))
            ;;
    esac
    mosquitto_pub -h mqtt -t $TOPIC -m $VALUE
done

