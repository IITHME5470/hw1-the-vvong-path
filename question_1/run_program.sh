#!/bin/bash

# Define directory paths
input_dir="./input"
output_dir="./output"

# Ensure input and output directories exist
if [ ! -d "$input_dir" ]; then
    echo "Input directory ($input_dir) not found. Exiting."
    exit 1
fi

if [ ! -d "$output_dir" ]; then
    echo "Output directory ($output_dir) not found. Creating it..."
    mkdir -p "$output_dir"
fi

# Prompt user for the value of n
read -p "Enter the value of n: " n

# Validate the input
if ! [[ "$n" =~ ^[0-9]+$ ]]; then
    echo "Invalid input. Please enter a positive integer for n."
    exit 1
fi

# Write n to input.in
echo "$n" > "$input_dir/input.in"
echo -e "Value of n written to $input_dir/input.in: $n\n"

# Compile the C program
gcc -o main_program main.c -lm
if [ $? -ne 0 ]; then
    echo "Compilation failed. Please check your C code."
    exit 1
fi
echo -e "Compilation successful.\n"

# Define file names based on convention
asc_file=$(printf "%s/array_%06d_asc.out" "$output_dir" "$n")
bin_file=$(printf "%s/array_%06d_bin.out" "$output_dir" "$n")

# Run the program for ASCII format (format_flag = 0)
echo "Running program with format_flag = 0 (ASCII output)..."
echo 0 | ./main_program > /dev/null 2>&1
if [ $? -ne 0 ]; then
    echo "Program execution failed for format_flag = 0."
    exit 1
fi

# Run the program for Binary format (format_flag = 1)
echo -e "\nRunning program with format_flag = 1 (Binary output)..."
echo 1 | ./main_program > /dev/null 2>&1
if [ $? -ne 0 ]; then
    echo "Program execution failed for format_flag = 1."
    exit 1
fi

echo -e "\nResults: "
# Check file sizes
if [ -f "$asc_file" ]; then
    asc_size=$(ls -lh "$asc_file" | awk '{print $5}')
    echo -e "Disc: Size of ASCII file ($asc_file): $asc_size"
else
    echo "ASCII file ($asc_file) not found."
fi

if [ -f "$bin_file" ]; then
    bin_size=$(ls -lh "$bin_file" | awk '{print $5}')
    echo "Disc: Size of Binary file ($bin_file): $bin_size"
else
    echo "Binary file ($bin_file) not found."
fi

echo -e "Memory: n * n * sizeof(double) = $n * $n * 8 B = $((n * n * 8)) B = $((n * n * 8 / (1024 * 1024)))M"

echo -e "\nScript execution completed."
