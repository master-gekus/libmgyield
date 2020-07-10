@echo off
set _vs=%1
set _ps=%2
set _vs=%_vs:"=%
set _ps=%_ps:"=%
echo "%_vs%: %_ps%"
call "C:\Program Files\Microsoft SDKs\Windows\v7.1\Bin\SetEnv.cmd" /x64
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x86_amd64
