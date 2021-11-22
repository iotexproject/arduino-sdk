#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

BOARDS=(
    "esp8266:esp8266:generic"
    "esp32:esp32:esp32"
    "arduino:samd:nano_33_iot"
)

EXAMPLES=$(ls ../examples)

if [ "$#" -eq 1 ]; then
    EXAMPLES=$1
fi

if [ "$#" -eq 2 ]; then
    EXAMPLES=$1
    BOARDS=$2
fi

echo "Building examples:"
echo "${EXAMPLES}"
echo "Boards:"
echo "${BOARDS}"

arduino-cli lib install "WiFiNINA" "FlashStorage" 1> /dev/null

for example in ${EXAMPLES[@]}; do
    echo -e "----------------------------------------------------------------------------" 
    echo -e "Example: ${example}:"
    for board in ${BOARDS[@]}; do
        RESULT=$(arduino-cli compile --fqbn ${board} ../examples/${example} --format json | jq '.success')
        if [ ${RESULT} == "true" ]; then
            echo -e "\t${GREEN}${board}: Success${NC}"
        else
            echo -e "\t${RED}${board}: Error${NC}"
        fi
    done
done

