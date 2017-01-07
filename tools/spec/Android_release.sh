#!bash

WORK_DIR=$(cd "$(dirname "$0")"; pwd)
BUILD_TYPE=$1
PROJ_NAME=$2
PROJ_PATH=$3
if test "x$BUILD_TYPE" = "x" || test "x$PROJ_NAME" = "x" || test "x$PROJ_PATH" = "x" ; then
    echo usage:
    echo   Android_release.sh BUILD_TYPE PROJ_NAME PROJ_PATH
    exit 1
fi

ZF_ROOT_PATH=$WORK_DIR/../..
ZF_TOOLS_PATH=$ZF_ROOT_PATH/tools

if test "$BUILD_TYPE" = "impl" ; then
    sh "$ZF_TOOLS_PATH/spec/Android/Android_release_impl.sh" ${PROJ_NAME} $PROJ_PATH
else
    sh "$ZF_TOOLS_PATH/spec/Android/Android_release_lib.sh" $PROJ_NAME $PROJ_PATH
fi

