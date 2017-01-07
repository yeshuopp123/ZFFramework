@echo off
setlocal

set WORK_DIR=%~dp0
set PROJ_NAME=%~1%
set PROJ_PATH=%~2%
set JAR_PROJ_DIR_NAME_OR_DUMMY=%~3%
set LIB_PROJ_DIR_NAME_OR_DUMMY=%~4%

if not defined PROJ_NAME goto :usage
if not defined PROJ_PATH goto :usage
if not defined JAR_PROJ_DIR_NAME_OR_DUMMY goto :usage
if not defined LIB_PROJ_DIR_NAME_OR_DUMMY goto :usage
goto :run
:usage
echo usage:
echo   release.bat PROJ_NAME PROJ_PATH JAR_PROJ_DIR_NAME_OR_DUMMY LIB_PROJ_DIR_NAME_OR_DUMMY
exit /b 1
:run

set ZF_ROOT_PATH=%WORK_DIR%\..\..\..
set ZF_TOOLS_PATH=%ZF_ROOT_PATH%\tools
set RELEASE_PATH=%ZF_ROOT_PATH%\_release\Android

if not "%JAR_PROJ_DIR_NAME_OR_DUMMY%" == "DUMMY" (
    call "%ZF_TOOLS_PATH%\spec\Android\jar_build.bat" %PROJ_NAME% "%PROJ_PATH%\zfproj\Android\%JAR_PROJ_DIR_NAME_OR_DUMMY%"
    call "%ZF_TOOLS_PATH%\spec\Android\jar_copy.bat" %PROJ_NAME% "%PROJ_PATH%\zfproj\Android\%JAR_PROJ_DIR_NAME_OR_DUMMY%" "%RELEASE_PATH%\module\%PROJ_NAME%\libs" >nul 2>&1
)

if not "%LIB_PROJ_DIR_NAME_OR_DUMMY%" == "DUMMY" (
    call "%ZF_TOOLS_PATH%\spec\Android\libs_build.bat" %PROJ_NAME% "%PROJ_PATH%\zfproj\Android\%LIB_PROJ_DIR_NAME_OR_DUMMY%"
    call "%ZF_TOOLS_PATH%\spec\Android\libs_copy.bat" %PROJ_NAME% "%PROJ_PATH%\zfproj\Android\%LIB_PROJ_DIR_NAME_OR_DUMMY%" "%RELEASE_PATH%\module\%PROJ_NAME%\libs" >nul 2>&1
)

call "%ZF_TOOLS_PATH%\spec\Android\res_copy.bat" "%PROJ_PATH%\zfres" "%RELEASE_PATH%\module\%PROJ_NAME%\assets\zfres" >nul 2>&1
call "%ZF_TOOLS_PATH%\spec\Android\res_fix.bat" "%RELEASE_PATH%\module\%PROJ_NAME%\assets\zfres" >nul 2>&1
call "%ZF_TOOLS_PATH%\common\copy_header.bat" "%PROJ_PATH%\zfsrc" "%RELEASE_PATH%\module\%PROJ_NAME%\libs\include" >nul 2>&1

