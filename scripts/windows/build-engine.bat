@echo off

::
:: Build the engine
::

setlocal EnableDelayedExpansion

call scripts\windows\internal\log.bat

:: Find all source files
pushd engine
set sources=
for /r %%f in (*.c) do (
	set sources=!sources! %%f
)
popd
echo !COMPILE_INFO! Files: %sources% !LOG_END!

:: Build the engine
set assembly=haunt
set cflags=-g -shared -Wvarargs -Wall -Werror -std=c17
set includes=-Iengine/src -Iengine/deps -Iengine/deps/glad/include
set linker=-luser32 -lgdi32 -lopengl32
set defines=-D_DEBUG -D_CRT_SECURE_NO_WARNINGS -DDLL_EXPORT
echo !COMPILE_INFO! Building %assembly%... !LOG_END!
call clang %sources% %cflags% -o bin/%assembly%.dll %defines% %includes% %linker%
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
