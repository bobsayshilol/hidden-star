#!/bin/bash

cd "${0%/*}"
export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:./lib/
./hiddenstar.mac

