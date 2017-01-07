@echo off
setlocal

set WORK_DIR=%~dp0
set SRC_PATH=%~1%
set DST_PATH=%~2%

if not defined SRC_PATH goto :usage
if not defined DST_PATH goto :usage
goto :run
:usage
echo usage:
echo   copy_check.bat SRC_PATH DST_PATH
exit /b 1
:run

if exist "%SRC_PATH%\*" (
    mkdir "%DST_PATH%" >nul 2>&1
    xcopy /s/e/y/r/h/d "%SRC_PATH%" "%DST_PATH%" >nul 2>&1
) else (
    for /f %%i in ("%SRC_PATH%") do (
        mkdir "%%~dpi" >nul 2>&1
        echo f | xcopy /y/r/h/d "%%~fi" "%DST_PATH%" >nul 2>&1
    )
)

