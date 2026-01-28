cmake_minimum_required(VERSION 3.16)
project(kernel ASM_NASM C)

# Disable standard libraries - this is a freestanding kernel
set(CMAKE_C_COMPILER gcc)
set(CMAKE_ASM_NASM_COMPILER nasm)
set(CMAKE_LINKER ld)

# Set up NASM for elf64 format
set(CMAKE_ASM_NASM_OBJECT_FORMAT elf64)
set(CMAKE_ASM_NASM_FLAGS "-f elf64")

# C compiler flags for freestanding 64-bit kernel
set(CMAKE_C_FLAGS "-ggdb -ffreestanding -m64 -fno-pie -no-pie")

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)

# Gather source files
file(GLOB_RECURSE C_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
file(GLOB_RECURSE ASM_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.asm")

# Create object library from assembly (must be linked first)
add_library(kernel_asm OBJECT ${ASM_SOURCES})

# Create object library from C sources
add_library(kernel_c OBJECT ${C_SOURCES})

# Custom command to create the raw binary kernel
# Assembly objects must come first in the link order
add_custom_target(kernel ALL
    COMMAND ${CMAKE_LINKER} -Ttext 0x8200 --oformat binary 
            -o ${CMAKE_CURRENT_SOURCE_DIR}/kernel 
            $<TARGET_OBJECTS:kernel_asm> $<TARGET_OBJECTS:kernel_c>
    DEPENDS kernel_asm kernel_c
    COMMENT "Linking kernel binary"
)

# Custom command to create the ELF kernel (for debugging)
add_custom_target(kernel_elf
    COMMAND ${CMAKE_LINKER} -Ttext 0x8200 
            -o ${CMAKE_CURRENT_SOURCE_DIR}/kernel.elf 
            $<TARGET_OBJECTS:kernel_asm> $<TARGET_OBJECTS:kernel_c>
    DEPENDS kernel_asm kernel_c
    COMMENT "Linking kernel ELF"
)

# Clean target
add_custom_target(clean_kernel
    COMMAND ${CMAKE_COMMAND} -E remove -f 
            ${CMAKE_CURRENT_SOURCE_DIR}/kernel 
            ${CMAKE_CURRENT_SOURCE_DIR}/kernel.elf
    COMMENT "Cleaning kernel files"
)
