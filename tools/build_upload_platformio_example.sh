# Usage:
# If port is specified, this will try to upload as well
# ./build_upload_platformio_example.sh <your-port>

export PLATFORMIO_LIB_EXTRA_DIRS=`pwd`/..
cd examples/platformio_example
if [ "$#" -eq 1 ]; then
    pio run --environment esp32 --target=upload --upload-port $1
else
    pio run --environment esp32
fi