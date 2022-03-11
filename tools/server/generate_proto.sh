#!/bin/bash

mkdir -p proto
cd proto

CLONE_REPOSITORIES=false

if [ "$#" -eq 1 ]; then
    if [ $1 == "--clone" ]; then
        CLONE_REPOSITORIES=true
    fi
fi

if [ ! -d "iotex-proto" ]; then
    if [ CLONE_REPOSITORIES=true ]; then
        git clone https://github.com/iotexproject/iotex-proto.git
    fi
fi

if [ ! -d "googleapis" ]; then
    if [ CLONE_REPOSITORIES=true ]; then
        git clone https://github.com/googleapis/googleapis.git
    fi
fi

protoc -Igoogleapis -Iiotex-proto --include_imports --descriptor_set_out=out/api.pb iotex-proto/proto/api/api.proto