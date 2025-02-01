@echo off

setlocal EnableDelayedExpansion

call scripts\windows\internal\log.bat

:: Check for required tools
echo !SETUP_INFO! Checking for required tools... !LOG_END!

:: git
call git --version >nul 2>&1
if %errorlevel% neq 0 (
	echo !SETUP_ERROR! Git is not installed. Please install Git and try again. Link: https://git-scm.com/downloads/win !LOG_END!
	goto exit_error
)
echo !SETUP_INFO! git found

:: clang
call clang --version >nul 2>&1
if %errorlevel% neq 0 (
	echo !SETUP_ERROR! Clang is not installed. Please install Clang and try again. Link: https://llvm.org/builds/ !LOG_END!
	goto exit_error
)
echo !SETUP_INFO! clang found !LOG_END!

:: Done checking for required tools
echo !SETUP_INFO! All required tools found !LOG_END!

:: Set up submodules
echo !SETUP_INFO! Setting up submodules... !LOG_END!
call git submodule update --init --recursive
if %errorlevel% neq 0 (
	echo !SETUP_ERROR! Failed to set up submodules. !LOG_END!
	goto exit_error
)
echo !SETUP_INFO! Submodules set up !LOG_END!

:: Create bin directory
if not exist bin (
	mkdir bin
	echo !SETUP_INFO! Created bin directory !LOG_END!
)

:: Done setting up
echo !SETUP_SUCCESS! Setup complete !LOG_END!

:exit
endlocal
exit /b 0

:exit_error
endlocal
exit /b 1
