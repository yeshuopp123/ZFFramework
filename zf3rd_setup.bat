@echo off
setlocal

set WORK_DIR=%~dp0
set ZF_ROOT_PATH=%WORK_DIR%
set ZF_TOOLS_PATH=%ZF_ROOT_PATH%\tools

call "%ZF_TOOLS_PATH%\common\zfsh_recursive.bat" "%ZF_ROOT_PATH%" "zfautoscript_zf3rd_setup*.zfsh"

