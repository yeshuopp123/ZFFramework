#!bash

WORK_DIR=$(cd "$(dirname "$0")"; pwd)
ZF_ROOT_PATH=$WORK_DIR/../..
ZF_TOOLS_PATH=$ZF_ROOT_PATH/tools
PROJ_ROOT=$1
PROJ_NAME=$2

if test "x$PROJ_ROOT" = "x" || test "x$PROJ_NAME" = "x" ; then
    echo usage:
    echo   cleanup_proj.sh PROJ_ROOT PROJ_NAME
    exit 1
fi

# ============================================================
# common
find "$PROJ_ROOT" -name "zfgensrc_*" -type f 2>/dev/null | xargs rm >/dev/null 2>&1

# ============================================================
# Windows
sh "$ZF_TOOLS_PATH/common/cleanup_Windows.sh" "$PROJ_ROOT/Windows" "$PROJ_NAME"

# ============================================================
# Android
sh "$ZF_TOOLS_PATH/common/cleanup_Android.sh" "$PROJ_ROOT/Android" "$PROJ_NAME"

# ============================================================
# iOS
sh "$ZF_TOOLS_PATH/common/cleanup_iOS.sh" "$PROJ_ROOT/iOS" "$PROJ_NAME"

# ============================================================
# Qt
sh "$ZF_TOOLS_PATH/common/cleanup_Qt.sh" "$PROJ_ROOT/Qt" "$PROJ_NAME"

exit 0

