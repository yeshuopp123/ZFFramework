#!bash

WORK_DIR=$(cd "$(dirname "$0")"; pwd)
PROJ_ROOT=$1
PROJ_NAME=$2

if test "x$PROJ_ROOT" = "x" || test "x$PROJ_NAME" = "x" ; then
    echo usage:
    echo   cleanup_Android.sh PROJ_ROOT PROJ_NAME
    exit 1
fi

rm -rf "$PROJ_ROOT/$PROJ_NAME/assets" >/dev/null 2>&1
rm -rf "$PROJ_ROOT/$PROJ_NAME/bin" >/dev/null 2>&1
rm -rf "$PROJ_ROOT/$PROJ_NAME/gen" >/dev/null 2>&1
rm -rf "$PROJ_ROOT/$PROJ_NAME/lib" >/dev/null 2>&1
rm -rf "$PROJ_ROOT/$PROJ_NAME/libs" >/dev/null 2>&1
rm -rf "$PROJ_ROOT/$PROJ_NAME/obj" >/dev/null 2>&1

exit 0

