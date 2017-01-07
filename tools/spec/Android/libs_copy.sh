#!bash

WORK_DIR=$(cd "$(dirname "$0")"; pwd)
PROJ_NAME=$1
PROJ_PATH=$2
DST_PATH=$3
if test "x$PROJ_NAME" = "x" || test "x$PROJ_PATH" = "x" || test "x$DST_PATH" = "x" ; then
    echo usage:
    echo   libs_copy.sh PROJ_NAME PROJ_PATH DST_PATH
    exit 1
fi

find "$DST_PATH" -name "*$PROJ_NAME.so" 2>/dev/null | xargs rm >/dev/null 2>&1
mkdir -p "$DST_PATH" >nul 2>&1
cp -rf "$PROJ_PATH/libs/." "$DST_PATH/" >/dev/null 2>&1
find "$DST_PATH" ! -name "*$PROJ_NAME.so" -type f 2>/dev/null | xargs rm >/dev/null 2>&1

