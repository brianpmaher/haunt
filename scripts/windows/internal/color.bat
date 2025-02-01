@echo off

::
:: Color codes for Windows cmd prompt
::
:: Credit to Michele Locati for the color codes (https://gist.github.com/mlocati/fdabcaeb8071d5c75a2d51712db24011#file-win10colors-cmd)
::

set SHOW_COLOR_EXAMPLES=0

for /F %%a in ('echo prompt $E ^| cmd') do (
	set "ESC=%%a"
)

:: Foreground colors
set "RED=%ESC%[31m"
set "GREEN=%ESC%[32m"
set "YELLOW=%ESC%[33m"
set "BLUE=%ESC%[34m"
set "MAGENTA=%ESC%[35m"
set "CYAN=%ESC%[36m"
set "WHITE=%ESC%[37m"
set "RESET=%ESC%[0m"

:: Examples
if %SHOW_COLOR_EXAMPLES% equ 1 (
	echo %RED%Red%RESET%
	echo %GREEN%Green%RESET%
	echo %YELLOW%Yellow%RESET%
	echo %BLUE%Blue%RESET%
	echo %MAGENTA%Magenta%RESET%
	echo %CYAN%Cyan%RESET%
	echo %WHITE%White%RESET%
)
