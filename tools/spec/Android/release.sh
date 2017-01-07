#!bash

WORK_DIR=$(cd "$(dirname "$0")"; pwd)
PROJ_NAME=$1
PROJ_PATH=$2
JAR_PROJ_DIR_NAME_OR_DUMMY=$3
LIB_PROJ_DIR_NAME_OR_DUMMY=$4
if test "x$PROJ_NAME" = "x"\
    || test "x$PROJ_PATH" = "x"\
    || test "x$JAR_PROJ_DIR_NAME_OR_DUMMY" = "x"\
    || test "x$LIB_PROJ_DIR_NAME_OR_DUMMY" = "x"\
    ; then
    echo usage:
    echo   release.sh PROJ_NAME PROJ_PATH JAR_PROJ_DIR_NAME_OR_DUMMY LIB_PROJ_DIR_NAME_OR_DUMMY
    exit 1
fi

ZF_ROOT_PATH=$WORK_DIR/../../..
ZF_TOOLS_PATH=$ZF_ROOT_PATH/tools
RELEASE_PATH=$ZF_ROOT_PATH/_release/Android

if test ! "$JAR_PROJ_DIR_NAME_OR_DUMMY" = "DUMMY" ; then
    sh "$ZF_TOOLS_PATH/spec/Android/jar_build.sh" $PROJ_NAME "$PROJ_PATH/zfproj/Android/$JAR_PROJ_DIR_NAME_OR_DUMMY"
    sh "$ZF_TOOLS_PATH/spec/Android/jar_copy.sh" $PROJ_NAME "$PROJ_PATH/zfproj/Android/$JAR_PROJ_DIR_NAME_OR_DUMMY" "$RELEASE_PATH/module/$PROJ_NAME/libs" >/dev/null 2>&1
fi

if "$LIB_PROJ_DIR_NAME_OR_DUMMY" != "DUMMY" (
    sh "$ZF_TOOLS_PATH/spec/Android/libs_build.sh" $PROJ_NAME "$PROJ_PATH/zfproj/Android/$LIB_PROJ_DIR_NAME_OR_DUMMY"
    sh "$ZF_TOOLS_PATH/spec/Android/libs_copy.sh" $PROJ_NAME "$PROJ_PATH/zfproj/Android/$LIB_PROJ_DIR_NAME_OR_DUMMY" "$RELEASE_PATH/module/$PROJ_NAME/libs" >/dev/null 2>&1
)

sh "$ZF_TOOLS_PATH/spec/Android/res_copy.sh" $PROJ_NAME "$PROJ_PATH/zfres" "$RELEASE_PATH/module/$PROJ_NAME/assets/zfres" >/dev/null 2>&1
sh "$ZF_TOOLS_PATH/spec/Android/res_fix.sh" "$RELEASE_PATH/module/$PROJ_NAME/assets/zfres" >/dev/null 2>&1
sh "$ZF_TOOLS_PATH/common/copy_header.sh" "$PROJ_PATH/zfsrc" "$RELEASE_PATH/module/$PROJ_NAME/libs/include" >/dev/null 2>&1

