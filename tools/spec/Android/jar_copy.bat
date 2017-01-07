@echo off
setlocal

set WORK_DIR=%~dp0
set PROJ_NAME=%~1%
set PROJ_PATH=%~2%
set DST_PATH=%~3%

if not defined PROJ_NAME goto :usage
if not defined PROJ_PATH goto :usage
if not defined DST_PATH goto :usage
goto :run
:usage
echo usage:
echo   jar_copy.bat PROJ_NAME PROJ_PATH DST_PATH
exit /b 1
:run

mkdir "%DST_PATH%" >nul 2>&1
del "%DST_PATH%\%PROJ_NAME%.jar" >nul 2>&1
copy "%PROJ_PATH%\bin\classes.jar" "%DST_PATH%\%PROJ_NAME%.jar"

