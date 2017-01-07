@echo off
setlocal

set WORK_DIR=%~dp0
set RES_PATH=%~1%
set DST_PATH=%~2%

if not defined RES_PATH goto :usage
if not defined DST_PATH goto :usage
goto :run
:usage
echo usage:
echo   res_copy.bat RES_PATH DST_PATH
exit /b 1
:run

mkdir "%DST_PATH%" >nul 2>&1
xcopy /s/e/y/r/h "%RES_PATH%" "%DST_PATH%"

