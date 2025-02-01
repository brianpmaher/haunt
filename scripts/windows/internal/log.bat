@echo off

::
:: Common log messages
::

set SHOW_LOG_EXAMPLES=0

call scripts\windows\internal\color.bat

:: Setup script log messages
set "SETUP_INFO=%WHITE%[SETUP-INFO]"
set "SETUP_ERROR=%RED%[SETUP-ERROR]"
set "SETUP_SUCCESS=%GREEN%[SETUP-SUCCESS]"

:: Compile script log messages
set "COMPILE_INFO=%WHITE%[COMPILE-INFO]"
set "COMPILE_ERROR=%RED%[COMPILE-ERROR]"
set "COMPILE_SUCCESS=%GREEN%[COMPILE-SUCCESS]"

:: Reset log messages
set "LOG_END=%RESET%"

:: Examples
if %SHOW_LOG_EXAMPLES% equ 1 (
	echo %SETUP_INFO% Message %RESET%
	echo %SETUP_ERROR% Message %RESET%
	echo %SETUP_SUCCESS% Message %RESET%
	echo %COMPILE_INFO% Message %RESET%
	echo %COMPILE_ERROR% Message %RESET%
	echo %COMPILE_SUCCESS% Message %RESET%
)
