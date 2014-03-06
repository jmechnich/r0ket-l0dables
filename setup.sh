#!/bin/sh

if ! [ -e arm-cs-tools/bin ]; then
    echo "Could not find arm-cs-tools binaries"
else
    export PATH=$PWD/arm-cs-tools/bin:$PATH
fi
