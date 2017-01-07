@echo off
setlocal

set WORK_DIR=%~dp0
set PROJ_NAME=%~1%
set PROJ_PATH=%~2%

if not defined PROJ_NAME goto :usage
if not defined PROJ_PATH goto :usage
goto :run
:usage
echo usage:
echo   libs_build.bat PROJ_NAME PROJ_PATH
exit /b 1
:run

set _OLD_DIR=%cd%
cd "%PROJ_PATH%"
call ndk-build -j2
cd "%_OLD_DIR%"

