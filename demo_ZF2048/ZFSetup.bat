@echo off
setlocal

set WORK_DIR=%~dp0
set ZF_ROOT_PATH=%WORK_DIR%\..\..\ZFFramework
set ZF_TOOLS_PATH=%ZF_ROOT_PATH%\tools
set PROJ_ROOT=%WORK_DIR%\..

if not exist "%ZF_TOOLS_PATH%" (
    rmdir /s/q "%ZF_ROOT_PATH%" >nul 2>&1
    git clone "https://github.com/ZFFramework/ZFFramework.git" "%ZF_ROOT_PATH%"
)

