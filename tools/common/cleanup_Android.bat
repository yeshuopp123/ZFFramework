@echo off
setlocal

set WORK_DIR=%~dp0
set PROJ_ROOT=%~1%
set PROJ_NAME=%~2%

if not defined PROJ_ROOT goto :usage
if not defined PROJ_NAME goto :usage
goto :run
:usage
echo usage:
echo   cleanup_Android.bat PROJ_ROOT PROJ_NAME
exit /b 1
:run

rem ============================================================
rem Android
rmdir /s/q "%PROJ_ROOT%\%PROJ_NAME%\assets" >nul 2>&1
rmdir /s/q "%PROJ_ROOT%\%PROJ_NAME%\bin" >nul 2>&1
rmdir /s/q "%PROJ_ROOT%\%PROJ_NAME%\gen" >nul 2>&1
rmdir /s/q "%PROJ_ROOT%\%PROJ_NAME%\lib" >nul 2>&1
rmdir /s/q "%PROJ_ROOT%\%PROJ_NAME%\libs" >nul 2>&1
rmdir /s/q "%PROJ_ROOT%\%PROJ_NAME%\obj" >nul 2>&1

exit /b 0

