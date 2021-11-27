#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

ENVS=(
    "nano_33_iot"
    "esp8266"
    "esp32"
)

EXAMPLES=$(ls ../examples)

if [ "$#" -eq 1 ]; then
    EXAMPLES=$1
fi

if [ "$#" -eq 2 ]; then
    EXAMPLES=$1
    ENVS=$2
fi

echo "Building examples:"
echo "${EXAMPLES}"
echo "Environments:"
echo "${ENVS}"

export PLATFORMIO_LIB_EXTRA_DIRS="~/Documents/Arduino/libraries"

for example in ${EXAMPLES[@]}; do
    echo -e "----------------------------------------------------------------------------" 
    echo -e "Example: ${example}:"
    cd ../examples/${example}
    
    for env in ${ENVS[@]}; do
        pio run --environment ${env} 1> /dev/null 2> /dev/null
        if [ $? -eq 0 ]; then
            echo -e "\t${GREEN}${env}: Success${NC}"
        else
            echo -e "\t${RED}${env}: Error${NC}"
        fi
    done

    cd ../../tools
done

