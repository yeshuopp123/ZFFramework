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
echo   Android_release.bat BUILD_TYPE PROJ_NAME PROJ_PATH
exit /b 1
:run

set ZF_ROOT_PATH=%WORK_DIR%\..\..
set ZF_TOOLS_PATH=%ZF_ROOT_PATH%\tools

if "%BUILD_TYPE%" == "impl" (
    call "%ZF_TOOLS_PATH%\spec\Android\Android_release_impl.bat" %PROJ_NAME% %PROJ_PATH%
) else (
    call "%ZF_TOOLS_PATH%\spec\Android\Android_release_lib.bat" %PROJ_NAME% %PROJ_PATH%
)

