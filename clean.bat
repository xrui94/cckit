@echo off
REM SGIR Clean Script (Windows)
REM Usage: clean.bat

echo.
echo ========================================
echo SGIR Clean Script (Windows)
echo ========================================
echo.

REM Clean build directory
if exist "build\local-windows" (
    echo Cleaning build directory...
    rmdir /s /q "build\local-windows"
    echo Build directory cleaned
) else (
    echo Build directory does not exist, nothing to clean
)

echo.
echo ========================================
echo Clean completed!
echo ========================================
echo.
