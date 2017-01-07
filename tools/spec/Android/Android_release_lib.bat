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
echo   Android_release_lib.bat PROJ_NAME PROJ_PATH
exit /b 1
:run

set ZF_ROOT_PATH=%WORK_DIR%\..\..\..
set ZF_TOOLS_PATH=%ZF_ROOT_PATH%\tools
set RELEASE_PATH=%ZF_ROOT_PATH%\_release\Android

call "%ZF_TOOLS_PATH%\common\file_exist.bat" "%RELEASE_PATH%\module\%PROJ_NAME%\libs" "*.so"
if "%errorlevel%" == "0" (
    goto :EOF
)

call "%ZF_TOOLS_PATH%\spec\Android\release.bat" %PROJ_NAME% %PROJ_PATH% DUMMY %PROJ_NAME%_jni

call "%ZF_TOOLS_PATH%\common\copy_check.bat" "%RELEASE_PATH%\module\%PROJ_NAME%" "%RELEASE_PATH%\all" >nul 2>&1

