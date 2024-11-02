#!/bin/bash
mkdir -p win-build && cd win-build
cmake -DCMAKE_TOOLCHAIN_FILE=../windows-toolchain.cmake .. && make
