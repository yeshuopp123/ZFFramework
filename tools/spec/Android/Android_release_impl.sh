#!bash

WORK_DIR=$(cd "$(dirname "$0")"; pwd)
PROJ_NAME=$1
PROJ_PATH=$2
if test "x$PROJ_NAME" = "x" || test "x$PROJ_PATH" = "x" ; then
    echo usage:
    echo   Android_release_lib.sh PROJ_NAME PROJ_PATH
    exit 1
fi

ZF_ROOT_PATH=$WORK_DIR/../../..
ZF_TOOLS_PATH=$ZF_ROOT_PATH/tools
RELEASE_PATH=$ZF_ROOT_PATH/_release/Android

sh "$ZF_TOOLS_PATH/common/file_exist.sh" "$RELEASE_PATH/module/$PROJ_NAME/libs" "*.jar"
if test "$?" = "0" ; then
    exit 0
fi

sh "$ZF_TOOLS_PATH/spec/Android/release.sh" $PROJ_NAME $PROJ_PATH $PROJ_NAME ${PROJ_NAME}_jni

sh "$ZF_TOOLS_PATH/common/copy_check.sh" "$RELEASE_PATH/module/$PROJ_NAME" "$RELEASE_PATH/all" >/dev/null 2>&1

