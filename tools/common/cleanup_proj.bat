@echo off
setlocal

set WORK_DIR=%~dp0
set ZF_ROOT_PATH=%WORK_DIR%\..\..
set ZF_TOOLS_PATH=%ZF_ROOT_PATH%\tools
set PROJ_ROOT=%~1%
set PROJ_NAME=%~2%

if not defined PROJ_ROOT goto :usage
if not defined PROJ_NAME goto :usage
goto :run
:usage
echo usage:
echo   cleanup_proj.bat PROJ_ROOT PROJ_NAME
exit /b 1
:run

rem ============================================================
rem common
del /s/q "%PROJ_ROOT%\zfgensrc_*" >nul 2>&1

rem ============================================================
rem Windows
call "%ZF_TOOLS_PATH%\common\cleanup_Windows.bat" "%PROJ_ROOT%\Windows" "%PROJ_NAME%"

rem ============================================================
rem Android
call "%ZF_TOOLS_PATH%\common\cleanup_Android.bat" "%PROJ_ROOT%\Android" "%PROJ_NAME%"

rem ============================================================
rem iOS
call "%ZF_TOOLS_PATH%\common\cleanup_iOS.bat" "%PROJ_ROOT%\iOS" "%PROJ_NAME%"

rem ============================================================
rem Qt
call "%ZF_TOOLS_PATH%\common\cleanup_Qt.bat" "%PROJ_ROOT%\Qt" "%PROJ_NAME%"

exit /b 0

