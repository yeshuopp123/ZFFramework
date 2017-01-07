@echo off
setlocal

set WORK_DIR=%~dp0
set ZF_ROOT_PATH=%WORK_DIR%\..\..
set ZF_TOOLS_PATH=%ZF_ROOT_PATH%\tools

if not exist "%ZF_ROOT_PATH%\_release\Android\all\libs" (
    call "%ZF_TOOLS_PATH%\release\release_Android.bat"
)

