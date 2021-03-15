@echo off
rem Public domain
rem http://unlicense.org/
rem Created by Grigore Stefan <g_stefan@yahoo.com>

set ACTION=%1
if "%1" == "" set ACTION=make

echo -^> %ACTION% quantum-script-extension-pixel32

goto StepX
:cmdX
%*
if errorlevel 1 goto cmdXError
goto :eof
:cmdXError
echo "Error: %ACTION%"
exit 1
:StepX

call :cmdX file-to-cs --touch=source/quantum-script-extension-pixel32.cpp --file-in=source/quantum-script-extension-pixel32.js --file-out=source/quantum-script-extension-pixel32.src --is-string --name=extensionPixel32Source
call :cmdX xyo-cc --mode=%ACTION% @build/source/quantum-script-extension-pixel32.static.compile
call :cmdX xyo-cc --mode=%ACTION% @build/source/quantum-script-extension-pixel32.dynamic.compile
