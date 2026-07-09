@echo off
REM CCKIT Build Script (Windows)
REM Usage: build.bat [config]
REM   config: Debug (default) or Release

setlocal EnableDelayedExpansion

REM Parse arguments
set "CONFIG=Debug"
if not "%1"=="" set "CONFIG=%1"

REM Validate configuration
if /I not "%CONFIG%"=="Debug" if /I not "%CONFIG%"=="Release" (
    echo Error: Invalid configuration '%CONFIG%'
    echo Usage: build.bat [Debug^|Release]
    exit /b 1
)

echo.
echo ========================================
echo CCKIT Build Script (Windows)
echo ========================================
echo Configuration: %CONFIG%
echo ========================================
echo.

REM Step 1: CMake Configure
echo [1/2] Configuring project...
cmake --preset local-windows -DCCKIT_BUILD_ALL=ON -DCCKIT_BUILD_TESTS=ON
if %ERRORLEVEL% neq 0 (
    echo.
    echo Error: CMake configuration failed
    exit /b 1
)

echo.
echo [1/2] CMake configuration successful
echo.

REM Step 2: Build Project
echo [2/2] Building project (%CONFIG%)...
cmake --build build/local-windows --config %CONFIG%
if %ERRORLEVEL% neq 0 (
    echo.
    echo Error: Build failed
    exit /b 1
)

echo.
echo ========================================
echo Build successful!
echo ========================================
echo Output directory: build\local-windows\bin\%CONFIG%
echo ========================================
echo.

endlocal
