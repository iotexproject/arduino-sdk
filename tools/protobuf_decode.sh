#!/bin/bash

protoc -I../src/protobuf_files --decode=ActionCore action.proto < encoded_actioncore.bin