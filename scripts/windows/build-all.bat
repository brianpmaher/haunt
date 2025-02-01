@echo off

::
:: Build the engine and editor
::

setlocal EnableDelayedExpansion

call scripts\windows\internal\log.bat

echo !COMPILE_INFO! Building all... !LOG_END!

:: Build the engine
call scripts\windows\build-engine.bat
if %errorlevel% neq 0 (
	goto exit_error
)

:: Build the editor
call scripts\windows\build-editor.bat
if %errorlevel% neq 0 (
	goto exit_error
)

:: Done building
echo !COMPILE_SUCCESS! Done building all !LOG_END!

:exit
endlocal
exit /b 0

:exit_error
endlocal
exit /b 1
