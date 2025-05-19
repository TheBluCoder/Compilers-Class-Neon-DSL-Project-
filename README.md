# CST8512 Compilers Course Project - Assignment 1

This repository contains the implementation of a Domain Specific Language (DSL) called Neon for the CST8512 Compilers course. The current branch (`assignment_1`) focuses on implementing the Vigenère cipher for encoding and decoding Neon source code.

## Getting Started

### Prerequisites

- Git
- CMake (version 3.10 or higher)
  - Windows: Download and install from [CMake's official website](https://cmake.org/download/)
  - Linux: Install using package manager
    ```bash
    sudo apt update
    sudo apt install cmake
    ```
  - Mac: Install using Homebrew
    ```bash
    brew install cmake
    ```
- C compiler:
  - Windows: CLion, MS Visual Studio 2022, or MinGW
  - Linux: GCC
  - Mac: Xcode Command Line Tools

### Installation

1. Clone the repository:
```bash
git clone https://github.com/TheBluCoder/cst85212.git
cd cst8512
```

2. Switch to the assignment_1 branch:
```bash
git checkout assignment_1
```

### Building the Project

#### Windows

You can build the project using any of these methods:

##### Using CLion (Recommended)
1. Open CLion
2. Select "Open" and choose the project directory
3. CLion will automatically detect the CMake project and configure it
4. Click the "Build" button or press Ctrl+F9

##### Using Visual Studio
```bash
# Create build directory
mkdir build
cd build

# Generate Visual Studio solution
cmake ..

# Build the project
cmake --build . --config Release
```

##### Using MinGW
```bash
# Create build directory
mkdir build
cd build

# Generate MinGW makefiles
cmake -G "MinGW Makefiles" ..

# Build the project
mingw32-make
```

#### Linux/Mac
```bash
# Create build directory
mkdir build
cd build

# Generate makefiles
cmake ..

# Build the project
make
```

After building, the executable will be located in the `build` directory.

## About Neon DSL

Neon is a custom Domain Specific Language with `.neo` file extension. This assignment implements the Vigenère cipher functionality to encode and decode Neon source code.

## Project Structure

- `Compilers.h` and `Compilers.c`: Core compiler components and utilities
- `Step1Coder.h` and `Step1Coder.c`: Implementation of the Vigenère cipher functionality
- `Main1Coder.c`: Main program entry point
- `test.neo`: Sample Neon DSL code for testing
- `out.txt`: Generated output file containing encoded/decoded results (gitignored)
- `CMakeLists.txt`: Build configuration file
- `.gitignore`: Specifies files to be ignored by git (including out.txt)

## Usage

The program requires exactly 4 command-line arguments to run:

### Windows
```bash
./Compilers.exe 1 <cypher-option> <input-file> <output-file>
```

### Linux/Mac
```bash
./Compilers 1 <cypher-option> <input-file> <output-file>
```

Required Parameters:
- First argument is always `1` (Assignment 1)
- `<cypher-option>`: Must be either:
  - `1`: Encode the input file
  - `0`: Decode the input file
- `<input-file>`: Path to your Neon source file (e.g., `test.neo`)
- `<output-file>`: Path where the encoded/decoded result will be stored (e.g., `out.txt`)

### Examples

To encode a file:
```bash
# Windows
Compilers.exe 1 1 test.neo out.txt

# Linux/Mac
./Compilers 1 1 test.neo out.txt
```

To decode a file:
```bash
# Windows
Compilers.exe 1 0 test.neo out.txt

# Linux/Mac
./Compilers 1 0 test.neo out.txt
```

### Error Handling

If any of the required parameters are missing, the program will display a usage message and exit:
```
Usage: Compilers [cypher=1|decypher=0] <input_file> <output_file>
```

The program will process the input file and generate the encoded/decoded output in the specified output file.

## Assignment 1 Features

- Implementation of Vigenère cipher algorithm
- Support for encoding and decoding Neon DSL code
- Basic data type definitions for the Neon language
- File I/O handling for `.neo` files

## Development

This is Assignment 1 of the CST8512 Compilers course at Algonquin College. The project is organized in branches:
- `assignment_1`: Current branch implementing Vigenère cipher (this branch)
- `assignment_2`: Future branch that will implement lexical analysis

## License

This is a private repository for educational purposes. 
