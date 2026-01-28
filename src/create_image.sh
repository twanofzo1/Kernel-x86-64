#!/bin/bash
# Creates the OS image by combining bootloader and kernel
# Usage: create_image.sh <boot_file> <kernel_file> <output_image>

BOOT_FILE="$1"
KERNEL_FILE="$2"
OUTPUT_IMAGE="$3"

# Copy bootloader to output
cp "$BOOT_FILE" "$OUTPUT_IMAGE"

# Calculate kernel size in sectors
kernel_size=$(wc -c < "$KERNEL_FILE")
kernel_sectors=$(( (kernel_size + 511) / 512 ))

# Patch kernel sector count at offset 2 in bootloader
printf '%02x' $kernel_sectors | xxd -r -p | dd of="$OUTPUT_IMAGE" bs=1 seek=2 count=1 conv=notrunc 2>/dev/null

# Append kernel to image
cat "$KERNEL_FILE" >> "$OUTPUT_IMAGE"

echo "Created $OUTPUT_IMAGE (kernel: $kernel_size bytes, $kernel_sectors sectors)"
