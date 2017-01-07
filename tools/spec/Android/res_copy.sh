#!bash

WORK_DIR=$(cd "$(dirname "$0")"; pwd)
RES_PATH=$1
DST_PATH=$2
if test "x$RES_PATH" = "x" || test "x$DST_PATH" = "x" ; then
    echo usage:
    echo   res_copy.sh RES_PATH DST_PATH
    exit 1
fi

mkdir -p "$DST_PATH" >/dev/null 2>&1
cp -rf "$RES_PATH/." "$DST_PATH/" >/dev/null 2>&1

