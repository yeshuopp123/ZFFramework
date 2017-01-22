#!bash

WORK_DIR=$(cd "$(dirname "$0")"; pwd)
ZF_ROOT_PATH=$WORK_DIR
ZF_TOOLS_PATH=$ZF_ROOT_PATH/tools
PROJ_ROOT=$WORK_DIR
CLEAN_WHAT=$1

if test "x-$CLEAN_WHAT" = "x-" || test "x-$CLEAN_WHAT" = "x-build" ; then
    sh "$ZF_TOOLS_PATH/common/zfsh_recursive.sh" "$PROJ_ROOT" "zfautoscript_cleanup*.zfsh"
fi

if test "x-$CLEAN_WHAT" = "x-" || test "x-$CLEAN_WHAT" = "x-zf3rd" ; then
    sh "$ZF_TOOLS_PATH/common/zfsh_recursive.sh" "$PROJ_ROOT" "zfautoscript_zf3rd_cleanup*.zfsh"
fi

rm -rf "$PROJ_ROOT/_release" >/dev/null 2>&1
rm -rf "$PROJ_ROOT/_tmp" >/dev/null 2>&1

