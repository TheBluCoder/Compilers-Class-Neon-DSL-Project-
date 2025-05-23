#!/usr/bin/bash

# Set umask for consistent file permissions
umask 022

# Check if CMake is installed
if ! command -v cmake &> /dev/null; then
    echo "Error: CMake is not installed. Please install CMake and try again."
    exit 1
fi

# Check if Make is installed
if ! command -v make &> /dev/null; then
    echo "Error: Make is not installed. Please install Make and try again."
    exit 1
fi

# Check if an argument is provided
if [[ -z "$1" ]]; then
    echo "Usage: $0 <encode|decode>"
    exit 1
fi

# Clean and create build directory, handling potential failures
if [ -d "build" ]; then
    echo "Removing existing build directory..."
    rm -rf build
fi

echo "Creating build directory..."
mkdir build || { echo "Error: Could not create build directory. Exiting."; exit 1; }

echo "Entering build directory..."
cd build || { echo "Error: Could not enter build directory. Exiting."; exit 1; }

# Run CMake and check for success
echo "-------- Running CMake --------"
cmake ..
if [ $? -ne 0 ]; then
    echo "Error: CMake configuration failed. Exiting."
    exit 1
fi

# Run Make and check for success
echo "-------- Running Make --------"
make
if [ $? -ne 0 ]; then
    echo "Error: Build failed. Exiting."
    exit 1
fi

# Execute the compiler based on the argument
echo "Executing Compilers..."
if [[ "$1" == "encode" ]]; then
    ./Compilers 1 1 ../test.neo ../out.txt
elif [[ "$1" == "decode" ]]; then
    ./Compilers 1 0 ../out.txt ../decoded.neo
else
    echo "Invalid argument: '$1'. Please use 'encode' or 'decode'."
    exit 1 # Exit with an error code for invalid arguments
fi

echo "Script finished."