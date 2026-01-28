#!/bin/bash

if test -f bin/os.img
then
    qemu-system-x86_64 -drive file=bin/os.img,format=raw
else
    sh build.sh
    qemu-system-x86_64 -drive file=bin/os.img,format=raw
fi
