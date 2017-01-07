@echo off
setlocal
setlocal enabledelayedexpansion

set WORK_DIR=%~dp0
set PROJ_GIT=%~1%
set DST_PATH=%~2%

if not defined PROJ_GIT goto :usage
if not defined DST_PATH goto :usage
goto :run
:usage
echo usage:
echo   git_check.bat PROJ_GIT DST_PATH
exit /b 1
:run

set _OLD_DIR=%cd%
set _TIMEOUT=86400
set _GIT_VALID=0

if exist "%DST_PATH%\.git" (
    set _GIT_PATH_DESIRED=%DST_PATH%\.git
    cd "%_GIT_PATH_DESIRED%" >nul 2>&1
    set _GIT_PATH_DESIRED=%CD%
    cd "%_OLD_DIR%"

    cd "%DST_PATH%" >nul 2>&1
    for /f "tokens=*" %%i in ('git rev-parse --show-toplevel 2^>nul') do set _GIT_PATH_EXIST=%%i\.git
    cd "%_OLD_DIR%"
    cd "%_GIT_PATH_EXIST%" >nul 2>&1
    set _GIT_PATH_EXIST=%CD%
    cd "%_OLD_DIR%"

    if "%_GIT_PATH_DESIRED%" == "%_GIT_PATH_EXIST%" (
        set _GIT_VALID=1
    )
)

if "%_GIT_VALID%" == "1" (
    call "%WORK_DIR%\timestamp_check.bat" "%DST_PATH%\.git" %_TIMEOUT%

    if not "!errorlevel!" == "0" (
        cd "%DST_PATH%"
        git checkout .
        git reset --hard
        git clean -xdf
        git pull
        set _SUCCESS=!errorlevel!
        cd "%_OLD_DIR%"

        if "!_SUCCESS!" == "0" (
            call "%WORK_DIR%\timestamp_save.bat" "%DST_PATH%\.git" %_TIMEOUT%
        )
    )
) else (
    rmdir /s/q "%DST_PATH%" >nul 2>&1
    mkdir "%DST_PATH%" >nul 2>&1
    git clone "%PROJ_GIT%" "%DST_PATH%"

    if "!errorlevel!" == "0" (
        call "%WORK_DIR%\timestamp_save.bat" "%DST_PATH%\.git" %_TIMEOUT%
    )
)

