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
echo   jar_build.bat PROJ_NAME PROJ_PATH
exit /b 1
:run

set _OLD_DIR=%cd%
cd "%PROJ_PATH%"
rmdir /s/q bin >nul 2>&1
call ant release
cd "%_OLD_DIR%"

