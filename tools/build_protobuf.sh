#!/bin/bash

# Needs protoc installed in system

../src/extern/nanopb/generator/nanopb_generator.py -I../src/protobuf_files -D../src/protobuf_files/c_files action.proto  blockchain.proto

## Post compile stepts
# 1. action.pb.h: Correct includes to src relative

## TODO Move .proto files out of src