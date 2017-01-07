@echo off
setlocal

set WORK_DIR=%~dp0
set RES_PATH=%~1%

if not defined RES_PATH goto :usage
goto :run
:usage
echo usage:
echo   res_fix.bat RES_PATH
exit /b 1
:run

>nul 2>&1 (
    for /f "delims=" %%i in ('dir /a-d /b /s %RES_PATH%') do (
        if not "%%~xi" == ".mp2" (
            del /s/q %%~nxi.mp2 >nul 2>&1
            ren %%i %%~nxi.mp2
        )
    )
)

