#!/bin/bash

if [ ! -f bin/os.img ]; then
    sh build.sh
fi

if [ -f bin/os.img ]; then
    qemu-system-x86_64 -drive file=bin/os.img,format=raw
else
    echo "Build failed. Cannot run OS."
    exit 1
fi
