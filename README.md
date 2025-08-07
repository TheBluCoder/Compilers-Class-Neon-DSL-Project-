# cst85212 Compilers Project (NEON DSL)
# Neon Compiler Project

## Overview

This is a comprehensive compiler project developed for the **CST 8152 - Compilers** course at Algonquin College. The project implements a complete frontend compiler for the **Neon** programming language, built incrementally through multiple assignments that each focus on a specific phase of the compilation process.

## Project Structure

The project is organized into multiple phases, each corresponding to a different assignment:

### Assignment Progression
- **Assignment 1**: Coder (Step 1) - Basic encoding/decoding functionality
- **Assignment 2**: Reader (Step 2) - File reading and buffer management
- **Assignment 3**: Scanner (Step 3) - Lexical analysis and tokenization
- **Assignment 4**: Parser (Step 4) - Syntax analysis and parsing
- **Assignment 5**: Writer (Step 5) - **Complete frontend compiler** with code generation and execution

## Current Status

**Assignment 5** contains the **complete frontend compiler** implementation, which includes all previous phases plus the final writer component that can execute Neon code.

## The Neon Language

Neon is a simple programming language with the following features:

### Data Types
- `number` - Numeric values (supports arithmetic operations)
- `text` - String literals
- `boolean` - True/False values

### Syntax Examples
```neon
# Variable declarations
number x = 2 + 4
number y = x * 2
text langName = "Neon"

# Output statements
output(x, console)
output("Hello World", console)
```

### Supported Operations
- Arithmetic operations: `+`, `-`, `*`, `/`
- Parenthesized expressions: `(x + y) * (x - y)`
- Variable assignments and references
- Console output with `output()` function

## Building and Running

### Prerequisites
- GCC compiler
- CMake (optional, for IDE support)
- Windows environment (for batch scripts)

### Quick Start

1. **Compile the project:**
   ```bash
   gcc compilers.c main1coder.c step1coder.c main2reader.c step2reader.c main3scanner.c step3scanner.c main4parser.c step4parser.c main5writer.c step5writer.c -o compilers
   ```

2. **Run the complete compiler (Assignment 5):**
   ```bash
   # Using the batch script
   A5.bat
   
   # Or directly
   compilers 5 testFiles/sm.neo
   ```

3. **Run individual phases:**
   ```bash
   # Coder (Phase 1)
   compilers 1 input_file
   
   # Reader (Phase 2)
   compilers 2 input_file
   
   # Scanner (Phase 3)
   compilers 3 input_file
   
   # Parser (Phase 4)
   compilers 4 input_file
   
   # Writer (Phase 5) - Complete frontend
   compilers 5 input_file
   ```

### Using CMake (Alternative)
```bash
mkdir build
cd build
cmake ..
make
./Compilers 5 ../testFiles/sm.neo
```

## File Structure

### Core Components
- `Compilers.c/h` - Main compiler entry point and shared definitions
- `Step1Coder.c/h` - Encoding/decoding functionality
- `Step2Reader.c/h` - File reading and buffer management
- `Step3Scanner.c/h` - Lexical analysis and tokenization
- `Step4Parser.c/h` - Syntax analysis and parsing
- `Step5Writer.c/h` - **Code generation and execution**

### Main Entry Points
- `Main1Coder.c` - Coder phase entry point
- `Main2Reader.c` - Reader phase entry point
- `Main3Scanner.c` - Scanner phase entry point
- `Main4Parser.c` - Parser phase entry point
- `Main5Writer.c` - **Writer phase entry point (complete compiler)**

### Test Files
- `testFiles/sm.neo` - Sample Neon source code
- `testFiles/sm.eneo` - Encoded test file

### Build Scripts
- `A5.bat` - Windows batch script for Assignment 5
- `CMakeLists.txt` - CMake configuration

## Features by Assignment

### Assignment 1: Coder
- Basic encoding/decoding functionality
- File format conversion

### Assignment 2: Reader
- File input/output operations
- Buffer management
- Memory-efficient file processing

### Assignment 3: Scanner
- Lexical analysis
- Token generation
- Error handling for lexical errors

### Assignment 4: Parser
- Syntax analysis
- Parse tree construction
- Grammar validation

### Assignment 5: Writer (Complete Frontend)
- **Variable management and storage**
- **Expression evaluation**
- **Arithmetic operations** (+, -, *, /)
- **String handling**
- **Console output functionality**
- **Code execution and interpretation**

## Key Features of the Complete Compiler (Assignment 5)

### Variable System
- Dynamic variable creation and management
- Support for multiple data types (numeric, string, boolean)
- Variable scope and lifetime management

### Expression Evaluation
- Arithmetic expression parsing and evaluation
- Operator precedence handling
- Parenthesized expression support
- Variable reference resolution

### Output System
- Console output functionality
- String literal output
- Variable value output
- Formatted numeric output

### Error Handling
- Robust error detection and reporting
- Graceful handling of invalid expressions
- Memory management and cleanup

## Testing

The project includes test files in the `testFiles/` directory:

- `sm.neo` - Contains valid Neon code for testing the complete compiler
- `sm.eneo` - Encoded version for testing earlier phases

### Sample Test Run
```bash
# Run the complete compiler on the test file
compilers 5 testFiles/sm.neo

# Expected output includes:
# - Variable assignments
# - Expression evaluations
# - Console output
# - Final variable state
```

## Development Notes

- **Language**: C
- **Compiler**: GCC (MS Visual Studio 2022 compatible)
- **Course**: CST 8152 - Compilers
- **Professor**: Paulo Sousa
- **Student**: Ikeoluwa Oladele
- **Term**: Summer 2025

## Branch Structure

The project uses Git branches to organize different assignments:
- `assignment2` - Reader implementation
- `assignment3` - Scanner implementation  
- `assignment4` - Parser implementation
- `assignment5` - **Complete frontend compiler** (current)
- `cleanup-writer` - Additional writer improvements
- `code-exam` - Exam-related code
- `test` - Testing branch

## Contributing

This is an academic project for CST 8152 at Algonquin College. The complete implementation is in Assignment 5, which provides a fully functional frontend compiler for the Neon programming language.

## License

This project is developed for educational purposes as part of the CST 8152 Compilers course at Algonquin College.

