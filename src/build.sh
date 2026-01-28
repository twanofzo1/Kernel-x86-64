#!/bin/bash

# Build everything using CMake
mkdir -p build
cd build
cmake ..
cmake --build .

if [ $? -eq 0 ]; then
    echo "Build finished successfully"
    echo "OS image created at: bin/os.img"
else
    echo "Build failed. See output for more info."
fi