#!bash

WORK_DIR=$(cd "$(dirname "$0")"; pwd)
PROJ_NAME=$1
PROJ_PATH=$2
if test "x$PROJ_NAME" = "x" || test "x$PROJ_PATH" = "x" ; then
    echo usage:
    echo   jar_build.sh PROJ_NAME PROJ_PATH
    exit 1
fi

_OLD_DIR=$(pwd)
cd "$PROJ_PATH"
rm -rf bin >/dev/null 2>&1
ant release
cd "$_OLD_DIR"

