#! /bin/bash

cmake -S . -B build &&
cmake --build build &&
cmake --build build --target test && ./build/bin/talibwasm_test
