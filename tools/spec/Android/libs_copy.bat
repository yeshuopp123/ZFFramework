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
echo   libs_copy.bat PROJ_NAME PROJ_PATH DST_PATH
exit /b 1
:run

del /s/q "%DST_PATH%\*%PROJ_NAME%.so" >nul 2>&1
mkdir "%DST_PATH%" >nul 2>&1
xcopy /s/e/y/r/h "%PROJ_PATH%\libs\*%PROJ_NAME%.so" "%DST_PATH%"

