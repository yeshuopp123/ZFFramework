@echo off
setlocal

set WORK_DIR=%~dp0
set BUILD_TYPE=%~1%
set PROJ_NAME=%~2%
set PROJ_PATH=%~3%

if not defined BUILD_TYPE goto :usage
if not defined PROJ_NAME goto :usage
if not defined PROJ_PATH goto :usage
goto :run
:usage
echo usage:
echo   Qt_Windows_release.bat BUILD_TYPE PROJ_NAME PROJ_PATH
exit /b 1
:run

set ZF_ROOT_PATH=%WORK_DIR%\..\..
set ZF_TOOLS_PATH=%ZF_ROOT_PATH%\tools
set QT_TYPE=Qt_Windows
set RELEASE_PATH=%ZF_ROOT_PATH%\_release\%QT_TYPE%

call "%ZF_TOOLS_PATH%\common\file_exist.bat" "%RELEASE_PATH%\module\%PROJ_NAME%\lib" "*%PROJ_NAME%*"
if "%errorlevel%" == "0" (
    goto :EOF
)

set _OLD_DIR=%cd%
mkdir "%ZF_ROOT_PATH%\_tmp\%QT_TYPE%\%PROJ_NAME%" >nul 2>&1
cd "%ZF_ROOT_PATH%\_tmp\%QT_TYPE%\%PROJ_NAME%"
del ".\*.dll" /f/s/q/a >nul 2>&1
del ".\*.a" /f/s/q/a >nul 2>&1
qmake "%PROJ_PATH%\zfproj\Qt\%PROJ_NAME%\%PROJ_NAME%.pro" -r -spec win32-g++
mingw32-make -j2
cd "%_OLD_DIR%"

call "%ZF_TOOLS_PATH%\common\copy_check.bat" "%RELEASE_PATH%\module\%PROJ_NAME%" "%RELEASE_PATH%\all" >nul 2>&1

