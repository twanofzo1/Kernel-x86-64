cmake_minimum_required(VERSION 3.16)
project(os NONE)

# Enable ASM_NASM and C languages for subdirectories
enable_language(ASM_NASM)
enable_language(C)

# Output directories
set(BOOTLOADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bootloader)
set(KERNEL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/kernel)
set(OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# Create bin directory if it doesn't exist
file(MAKE_DIRECTORY ${OUTPUT_DIR})

# ============================================================================
# Bootloader - assemble boot.asm to raw binary
# ============================================================================
add_custom_command(
    OUTPUT ${OUTPUT_DIR}/boot
    COMMAND nasm -I${BOOTLOADER_DIR}/ -o ${OUTPUT_DIR}/boot ${BOOTLOADER_DIR}/boot.asm
    DEPENDS ${BOOTLOADER_DIR}/boot.asm
            ${BOOTLOADER_DIR}/real_mode/elevate.asm
            ${BOOTLOADER_DIR}/real_mode/gdt.asm
            ${BOOTLOADER_DIR}/real_mode/load.asm
            ${BOOTLOADER_DIR}/real_mode/print.asm
            ${BOOTLOADER_DIR}/real_mode/print_hex.asm
            ${BOOTLOADER_DIR}/protected_mode/clear.asm
            ${BOOTLOADER_DIR}/protected_mode/detect_lm.asm
            ${BOOTLOADER_DIR}/protected_mode/elevate.asm
            ${BOOTLOADER_DIR}/protected_mode/gdt.asm
            ${BOOTLOADER_DIR}/protected_mode/init_pt.asm
            ${BOOTLOADER_DIR}/protected_mode/print.asm
            ${BOOTLOADER_DIR}/long_mode/clear.asm
            ${BOOTLOADER_DIR}/long_mode/print.asm
    COMMENT "Assembling bootloader"
    VERBATIM
)

add_custom_target(bootloader DEPENDS ${OUTPUT_DIR}/boot)

# ============================================================================
# Kernel - compile C and ASM sources, link to raw binary
# ============================================================================

# Set up compilers
set(CMAKE_C_COMPILER gcc)
set(CMAKE_ASM_NASM_COMPILER nasm)
set(CMAKE_LINKER ld)

# NASM flags for kernel (elf64 format)
set(CMAKE_ASM_NASM_OBJECT_FORMAT elf64)
set(CMAKE_ASM_NASM_COMPILE_OBJECT "<CMAKE_ASM_NASM_COMPILER> -f elf64 -o <OBJECT> <SOURCE>")

# C compiler flags for freestanding 64-bit kernel
set(CMAKE_C_FLAGS "-ggdb -ffreestanding -m64 -fno-pie -no-pie -c")

# Gather kernel source files
file(GLOB_RECURSE KERNEL_C_SOURCES "${KERNEL_DIR}/src/*.c")
file(GLOB KERNEL_ASM_SOURCES "${KERNEL_DIR}/*.asm")

# Create object files from assembly (must be linked first)
add_library(kernel_asm OBJECT ${KERNEL_ASM_SOURCES})
target_include_directories(kernel_asm PRIVATE ${KERNEL_DIR}/inc)

# Create object files from C sources
add_library(kernel_c OBJECT ${KERNEL_C_SOURCES})
target_include_directories(kernel_c PRIVATE ${KERNEL_DIR}/inc)

# Link kernel to raw binary
add_custom_command(
    OUTPUT ${OUTPUT_DIR}/kernel
    COMMAND ld -Ttext 0x8200 --oformat binary 
            -o ${OUTPUT_DIR}/kernel 
            $<TARGET_OBJECTS:kernel_asm> $<TARGET_OBJECTS:kernel_c>
    DEPENDS kernel_asm kernel_c
    COMMENT "Linking kernel binary"
    VERBATIM
)

add_custom_target(kernel DEPENDS ${OUTPUT_DIR}/kernel)

# Link kernel ELF for debugging
add_custom_command(
    OUTPUT ${OUTPUT_DIR}/kernel.elf
    COMMAND ld -Ttext 0x8200 
            -o ${OUTPUT_DIR}/kernel.elf 
            $<TARGET_OBJECTS:kernel_asm> $<TARGET_OBJECTS:kernel_c>
    DEPENDS kernel_asm kernel_c
    COMMENT "Linking kernel ELF (for debugging)"
    VERBATIM
)

add_custom_target(kernel_elf DEPENDS ${OUTPUT_DIR}/kernel.elf)

# ============================================================================
# OS Image - combine bootloader and kernel
# ============================================================================
add_custom_command(
    OUTPUT ${OUTPUT_DIR}/os.img
    COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/create_image.sh 
            ${OUTPUT_DIR}/boot 
            ${OUTPUT_DIR}/kernel 
            ${OUTPUT_DIR}/os.img
    DEPENDS bootloader kernel ${CMAKE_CURRENT_SOURCE_DIR}/create_image.sh
    COMMENT "Creating OS image"
)

add_custom_target(os_image ALL DEPENDS ${OUTPUT_DIR}/os.img)

# ============================================================================
# Run target - launch in QEMU
# ============================================================================
add_custom_target(run
    COMMAND qemu-system-x86_64 -drive format=raw,file=${OUTPUT_DIR}/os.img
    DEPENDS os_image
    COMMENT "Running OS in QEMU"
)

# ============================================================================
# Clean target
# ============================================================================
add_custom_target(clean_all
    COMMAND ${CMAKE_COMMAND} -E remove -f 
            ${OUTPUT_DIR}/boot
            ${OUTPUT_DIR}/kernel
            ${OUTPUT_DIR}/kernel.elf
            ${OUTPUT_DIR}/os.img
    COMMENT "Cleaning all build artifacts"
)
