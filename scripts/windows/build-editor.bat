@echo off

::
:: Build the editor
::

setlocal EnableDelayedExpansion

call scripts\windows\internal\log.bat

:: Find all source files
pushd editor
set sources=
for /r %%f in (*.c) do (
	set sources=!sources! %%f
)
popd
echo !COMPILE_INFO! Files: %sources% !LOG_END!

:: Build the editor
set assembly=editor
set cflags=-g -Wall -Werror
set includes=-Ieditor/src -Iengine/src
set linker=-Lbin -lhaunt.lib
set defines=-D_DEBUG -DDLL_IMPORT
echo !COMPILE_INFO! Building %assembly%... !LOG_END!
call clang %sources% %cflags% -o bin/%assembly%.exe %defines% %includes% %linker%
if %errorlevel% neq 0 (
	echo !COMPILE_ERROR! Failed to build %assembly% !LOG_END!
	goto exit_error
)

:: Done building
echo !COMPILE_SUCCESS! Done building %assembly% !LOG_END!

:exit
endlocal
exit /b 0

:exit_error
endlocal
exit /b 1
