@echo off
setlocal enabledelayedexpansion

REM Check if CMake is installed
where cmake >nul 2>nul
if !errorlevel! neq 0 (
    echo Error: CMake is not installed or not in PATH. Please install CMake and try again.
    exit /b 1
)

REM Check if Make is installed
where make >nul 2>nul
if !errorlevel! neq 0 (
    echo Error: Make is not installed or not in PATH. Please install Make and try again.
    exit /b 1
)

REM Check if an argument is provided
if "%~1"=="" (
    echo Usage: %0 ^<encode^|decode^>
    exit /b 1
)

REM Clean and create build directory, handling potential failures
if exist "build" (
    echo Removing existing build directory...
    rmdir /s /q "build"
    if !errorlevel! neq 0 (
        echo Error: Could not remove existing build directory. Exiting.
        exit /b 1
    )
)

echo Creating build directory...
mkdir "build"
if !errorlevel! neq 0 (
    echo Error: Could not create build directory. Exiting.
    exit /b 1
)

echo Entering build directory...
cd "build"
if !errorlevel! neq 0 (
    echo Error: Could not enter build directory. Exiting.
    exit /b 1
)

REM Run CMake and check for success
echo -------- Running CMake --------
cmake ..
if !errorlevel! neq 0 (
    echo Error: CMake configuration failed. Exiting.
    exit /b 1
)

REM Run Make and check for success
echo -------- Running Make --------
make
if !errorlevel! neq 0 (
    echo Error: Build failed. Exiting.
    exit /b 1
)

REM Execute the compiler based on the argument
echo Executing Compilers...
if /i "%~1"=="encode" (
    .\Compilers.exe 1 1 ..\test.neo ..\out.txt
) else if /i "%~1"=="decode" (
    .\Compilers.exe 1 0 ..\out.txt ..\decoded.neo
) else (
    echo Invalid argument: '%~1'. Please use 'encode' or 'decode'.
    exit /b 1
)

echo Script finished.
pause
