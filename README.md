# CST8512 Compilers Course Project - Assignment 1: Neo Compiler

This repository contains the implementation of a Domain Specific Language (DSL) called Neo for the CST8512 Compilers course. The current branch (`assignment_1`) focuses on implementing the Vigenère cipher for encoding and decoding Neo source code.

Neo is a domain-specific language designed for AI model integration and document processing.

## Project Structure

```
src/
├── Compilers.c          # Main compiler implementation
├── Compilers.h          # Compiler header file
├── Step1Coder.c         # Step 1 coding implementation (Vigenère cipher)
├── Step1Coder.h         # Step 1 coding header
├── Main1Coder.c         # Main coder implementation
├── CMakeLists.txt       # CMake build configuration
├── run.sh               # Recommended build script (Linux/Mac/WSL)
├── run.bat              # Windows build script
├── test.neo             # Sample Neo source file
└── out.txt              # Example output file (gitignored)

```

## Prerequisites

Before building and running the compiler, ensure you have the following installed:

- **Git**
- **CMake** (version 3.10 or higher)
- **Make** build system
- **GCC** or compatible C compiler

### Installing Prerequisites

**Ubuntu/Debian:**
```bash
sudo apt update
sudo apt install git cmake build-essential
```

**Windows:**
- Install Git from https://git-scm.com/
- Install CMake from https://cmake.org/download/
- Install MinGW-w64, Visual Studio Build Tools, or CLion
- Ensure `cmake` and `make` are in your PATH

**macOS:**
```bash
# Install Xcode Command Line Tools
xcode-select --install

# Install Homebrew (if not already installed)
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install dependencies
brew install git cmake
```

## Getting Started

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

3. Navigate to the source directory:
```bash
cd src
```

## Building and Running (Recommended Method)

### Using the Shell Script (RECOMMENDED - TESTED)

The easiest and most reliable way to build and run the compiler is using the provided shell script:

**Linux/Mac/WSL:**
```bash
chmod +x run.sh

# To encode a .neo file to compiled output:
./run.sh encode

# To decode compiled output back to .neo format:
./run.sh decode
```

**Windows:**
```cmd
# To encode a .neo file to compiled output:
run.bat encode

# To decode compiled output back to .neo format:
run.bat decode
```

These scripts will:
1. Check for required dependencies (CMake, Make)
2. Clean any existing build directory
3. Create a new build directory
4. Run CMake configuration
5. Build the project using Make
6. Execute the compiler with appropriate arguments

The scripts support two operations:
- **encode**: Compiles `test.neo` → `out.txt`
- **decode**: Decompiles `out.txt` → `decoded.neo`

## Alternative Build Methods

While the shell/batch scripts are the recommended approach, you can also build the project manually using the following methods:

### Method 1: Using CLion (Windows)
1. Open CLion
2. Select "Open" and choose the project directory
3. CLion will automatically detect the CMake project and configure it
4. Click the "Build" button or press Ctrl+F9

### Method 2: Using Visual Studio (Windows)
```bash
# Create build directory
mkdir build
cd build

# Generate Visual Studio solution
cmake ..

# Build the project
cmake --build . --config Release
```

### Method 3: Using MinGW (Windows)
```bash
# Create build directory
mkdir build
cd build

# Generate MinGW makefiles
cmake -G "MinGW Makefiles" ..

# Build the project
mingw32-make
```

### Method 4: Manual CMake Build (Linux/Mac)
```bash
mkdir build
cd build
cmake ..
make

# Run manually:
./Compilers 1 1 ../test.neo ../out.txt     # encode
./Compilers 1 0 ../out.txt ../decoded.neo  # decode
```

### Method 5: Direct GCC Compilation
```bash
gcc -o Compilers Compilers.c Step1Coder.c Main1Coder.c

# Run:
./Compilers 1 1 test.neo out.txt     # encode
./Compilers 1 0 out.txt decoded.neo  # decode
```

## Usage

The compiler accepts the following command-line arguments:
```
./Compilers <step> <mode> <input_file> <output_file>
```

Where:
- `step`: Compilation step (1 for Step 1 processing/Assignment 1)
- `mode`: 1 for encode, 0 for decode
- `input_file`: Source file to process
- `output_file`: Destination file for output

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

## About Neo DSL

Neo is a Domain Specific Language with `.neo` file extension designed for AI model integration and document processing. The project includes a sample Neo program (`test.neo`) that demonstrates:
- Loading AI models (Gemini, Claude, OpenAI-TTS)
- Document processing and text extraction
- Audio file handling
- Model-specific operations
- Output generation and caching

## Assignment 1 Features

- Implementation of Vigenère cipher algorithm for encoding and decoding
- Support for encoding and decoding Neo DSL code
- Basic data type definitions for the Neo language
- File I/O handling for `.neo` files
- Cross-platform build support (Windows, Linux, macOS)

## Troubleshooting

**Build Failures:**
- Ensure CMake and Make are properly installed and in PATH
- Check that you have a compatible C compiler installed
- Try cleaning the build directory: `rm -rf build` (Linux/Mac) or `rmdir /s build` (Windows)

**Permission Issues (Linux/Mac):**
```bash
chmod +x run.sh
```

**Missing Dependencies:**
- The scripts will check for CMake and Make and provide error messages if missing
- Follow the prerequisite installation instructions above

## Development

This is Assignment 1 of the CST8512 Compilers course at Algonquin College. The project is organized in branches:
- `assignment_1`: Current branch implementing Vigenère cipher (this branch)
- `assignment_2`: Future branch that will implement lexical analysis

## Author

Ikeoluwa Oladele - Compilers Assignment 1

## License

This is a private repository for educational purposes.

---

**Note:** The shell script (`run.sh`) and batch script (`run.bat`) are the thoroughly tested and recommended methods for building and running this project. Other build methods are provided for reference but may require additional configuration.
