#!/bin/bash

find ../src -path ../src/extern -prune -name "*.cpp" -o -name "*.c" -o -name "*.h"  | xargs -I {} clang-format -style=file -i {}