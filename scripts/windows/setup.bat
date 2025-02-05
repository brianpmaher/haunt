@echo off

setlocal EnableDelayedExpansion

call scripts\windows\internal\log.bat

:: Check for required tools
echo !SETUP_INFO! Checking for required tools... !LOG_END!

:: git
call git --version >nul 2>&1
if %errorlevel% neq 0 (
	echo !SETUP_ERROR! git is not installed. Please install Git and try again. Link: https://git-scm.com/downloads/win !LOG_END!
	goto exit_error
)
echo !SETUP_INFO! git found

:: cl
call cl >nul 2>&1
if %errorlevel% neq 0 (
	echo !SETUP_ERROR! Visual C compiler not found. If you don't have visual studio installed install it. ^
		Link: https://visualstudio.microsoft.com/downloads/ ^
		If you do have it installed, run vcvarsll.bat x64 and try again. !LOG_END!
)
echo !SETUP_INFO! cl found !LOG_END!

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
exit /b 0

:exit_error
exit /b 1
