#!/bin/bash

rm -rf ../build
mkdir ../build
cd ../build
cmake -DUNIT_TEST=ON ../
make