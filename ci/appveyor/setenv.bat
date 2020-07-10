@echo off
set _vs=%1
set _ps=%2
set _vs=%_vs:"=%
set _ps=%_ps:"=%
echo "%_vs%:  %_ps%"
if /i "%_vs%" == "Visual Studio 2015" (
  if /i "%_ps%" == "x64" (
    call "C:\Program Files\Microsoft SDKs\Windows\v7.1\Bin\SetEnv.cmd" /x64
    call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x86_amd64
    goto done
  )
  if /i "%_ps%" == "x86" (
    call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x86
    goto done
  )
  echo Unsupported platform: %_ps%
  goto error
)

if /i "%_vs%" == "Visual Studio 2017" (
  if /i "%_ps%" == "x64" (
    call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars64.bat"
    goto done
  )
  if /i "%_ps%" == "x86" (
    call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars32.bat"
    goto done
  )
  echo Unsupported platform: %_ps%
  goto error
)

echo Version of Visual studio is not supported: %_vs%
:error
exit 1

:done
