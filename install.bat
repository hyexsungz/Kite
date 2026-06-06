:: This file installs the Kite executable to the system PATH on Windows.

@echo off

echo Installing Kite...

mkdir C:\Kite\bin

copy kite.exe C:\Kite\bin\kite.exe

setx PATH "%PATH%;C:\Kite\bin" /M

echo Done.
echo Restart terminal then run: kite
pause
