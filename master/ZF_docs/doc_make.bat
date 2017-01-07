@echo off
setlocal

set WORK_DIR=%~dp0

set OUTPUT_PATH=%WORK_DIR%\..\..\_tmp\DoxygenDoc

rd /s/q %OUTPUT_PATH% >nul 2>&1
mkdir %OUTPUT_PATH% >nul 2>&1

set _OLD_DIR=%cd%
cd %WORK_DIR%\Doxygen
.\doxygen.exe
cd "%_OLD_DIR%"

set _OLD_DIR=%cd%
cd %OUTPUT_PATH%\html
%WORK_DIR%\Doxygen\hhc.exe index.hhp
cd "%_OLD_DIR%"

move %OUTPUT_PATH%\html\index.chm %OUTPUT_PATH%\ZFFramework_doc.chm
del /q %OUTPUT_PATH%\html\index.hhc >nul 2>&1
del /q %OUTPUT_PATH%\html\index.hhk >nul 2>&1
del /q %OUTPUT_PATH%\html\index.hhp >nul 2>&1

