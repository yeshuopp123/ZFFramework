#!bash

WORK_DIR=$(cd "$(dirname "$0")"; pwd)
PROJ_NAME=$1
PROJ_PATH=$2
DST_PATH=$3
if test "x$PROJ_NAME" = "x" || test "x$PROJ_PATH" = "x" || test "x$DST_PATH" = "x" ; then
    echo usage:
    echo   jar_copy.sh PROJ_NAME PROJ_PATH DST_PATH
    exit 1
fi

mkdir -p "$DST_PATH" >/dev/null 2>&1
rm "$DST_PATH/$PROJ_NAME.jar" >/dev/null 2>&1
cp "$PROJ_PATH/bin/classes.jar" "$DST_PATH/$PROJ_NAME.jar"

