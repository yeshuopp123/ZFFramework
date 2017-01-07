#!bash

WORK_DIR=$(cd "$(dirname "$0")"; pwd)
PROJ_NAME=$1
PROJ_PATH=$2
if test "x$PROJ_NAME" = "x" || test "x$PROJ_PATH" = "x" ; then
    echo usage:
    echo   libs_build.sh PROJ_NAME PROJ_PATH
    exit 1
fi

_OLD_DIR=$(pwd)
cd "$PROJ_PATH"
ndk-build -j2
cd "$_OLD_DIR"

