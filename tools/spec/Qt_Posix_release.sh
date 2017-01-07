#!bash

WORK_DIR=$(cd "$(dirname "$0")"; pwd)
BUILD_TYPE=$1
PROJ_NAME=$2
PROJ_PATH=$3
if test "x$BUILD_TYPE" = "x" || test "x$PROJ_NAME" = "x" || test "x$PROJ_PATH" = "x" ; then
    echo usage:
    echo   Qt_Posix_release.sh BUILD_TYPE PROJ_NAME PROJ_PATH
    exit 1
fi

ZF_ROOT_PATH=$WORK_DIR/../..
ZF_TOOLS_PATH=$ZF_ROOT_PATH/tools
QT_TYPE=Qt_Posix
RELEASE_PATH=$ZF_ROOT_PATH/_release/$QT_TYPE

sh "$ZF_TOOLS_PATH/common/file_exist.sh" "$RELEASE_PATH/module/$PROJ_NAME/lib" "*$PROJ_NAME*"
if test "$?" = "0" ; then
    exit 0
fi

_OLD_DIR=$(pwd)
mkdir -p "$ZF_ROOT_PATH/_tmp/$QT_TYPE/$PROJ_NAME" >/dev/null 2>&1
cd "$ZF_ROOT_PATH/_tmp/$QT_TYPE/$PROJ_NAME"
find . -name "*.a.*" | xargs rm
find . -name "*.so.*" | xargs rm
find . -name "*.dylib.*" | xargs rm
qmake "$PROJ_PATH/zfproj/Qt/$PROJ_NAME/$PROJ_NAME.pro" -r -spec linux-g++
make -j2
cd "$_OLD_DIR"

sh "$ZF_TOOLS_PATH/common/copy_check.sh" "$RELEASE_PATH/module/$PROJ_NAME" "$RELEASE_PATH/all" >/dev/null 2>&1

